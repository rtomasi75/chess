#include "SE_THREAD.h"
#include "SE_DISPATCHER.h"
#include <cstring>

#define RETENTIONTHRESHOLD_DROWSY 100
#define RETENTIONTHRESHOLD_SLEEPING 300

#define RETENTIONDELAY_SLEEP 0
#define RETENTIONDELAY_HIBERNATE 1

static inline void THREAD_HandleIdle(SE_THREAD* pThread)
{
	const SE_RETENTIONSTATE oldState = pThread->RetentionState.load();
	switch (pThread->RetentionState)
	{
	case RETENTIONSTATE_AWAKE:
		if (pThread->CountIdleTicks++ >= RETENTIONTHRESHOLD_DROWSY)
		{
			SE_RETENTIONSTATE expected = RETENTIONSTATE_AWAKE;
			if (pThread->RetentionState.compare_exchange_strong(expected, RETENTIONSTATE_DROWSY))
			{
				CM_COUNT_INC(pThread->RetentionTransitions);
			}
		}
		break;
	case RETENTIONSTATE_DROWSY:
		CM_YIELD();
		if (pThread->CountIdleTicks++ >= RETENTIONTHRESHOLD_SLEEPING)
		{
			SE_RETENTIONSTATE expected = RETENTIONSTATE_DROWSY;
			if (pThread->RetentionState.compare_exchange_strong(expected, RETENTIONSTATE_SLEEPING))
			{
				CM_COUNT_INC(pThread->RetentionTransitions);
			}
		}
		CM_COUNT_INC(pThread->YieldCount);
		break;
	case RETENTIONSTATE_SLEEPING:
		CM_SLEEP(RETENTIONDELAY_SLEEP);
		CM_COUNT_INC(pThread->SleepCount);
		break;
	case RETENTIONSTATE_HIBERNATING:
		CM_SLEEP(RETENTIONDELAY_HIBERNATE);
		break;
	}
#ifdef SEARCH_TRACE_THREAD
	if (pThread->RetentionState != oldState)
		THREAD_TRACE("THREAD %d: State = %d | IdleTicks = %u\n", pThread->ThreadId, (int)pThread->RetentionState, (unsigned)pThread->CountIdleTicks);
#endif
}

void THREAD_WakeUp(SE_THREAD* pThread)
{
	const SE_RETENTIONSTATE oldState = pThread->RetentionState.exchange(RETENTIONSTATE_AWAKE);
	if (oldState != RETENTIONSTATE_AWAKE)
	{
		if (oldState != RETENTIONDELAY_HIBERNATE)
		{
			CM_COUNT_INC(pThread->RetentionTransitions);
			CM_COUNT_INC(pThread->WakeCount);
		}
		THREAD_TRACE("THREAD %d: Wake requested (was state %d)\n", pThread->ThreadId, pThread->RetentionState);
		pThread->CountIdleTicks = 0;
	}
}

void THREAD_Hibernate(SE_THREAD* pThread)
{
	const SE_RETENTIONSTATE oldState = pThread->RetentionState.exchange(RETENTIONSTATE_HIBERNATING);
	if (oldState != RETENTIONSTATE_HIBERNATING)
	{
		THREAD_TRACE("THREAD %d: Hibernation requested (was state %d)\n", pThread->ThreadId, pThread->RetentionState);
		pThread->CountIdleTicks = 0;
	}
}

static void THREAD_MainLoop(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread, const MG_MOVEGEN* pMoveGen)
{
	THREAD_TRACE("THREAD: Thread with ID %u entering main loop.", pThread->ThreadId);
	char nameBuffer[32];
	std::memset(nameBuffer, 0, 32);
	std::snprintf(nameBuffer, 32, "Dispatcher thread %u", pThread->ThreadId);
	CM_SetCurrentThreadName(nameBuffer);
	while (!pThread->TerminationRequested)
	{
		if (CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags))
		{
			ASSERT(pThread->StateMachine != nullptr);
			THREAD_TRACE("THREAD: Thread with ID %u activating.", pThread->ThreadId);
			THREAD_WakeUp(pThread);
			CONTROLFLAGS_SET_RUNNING(pThread->ControlFlags);
			DISPATCHER_SignalRunning(pDispatcher, pThread);
			pThread->StateMachine(pThread, pMoveGen);
			ASSERT(pThread->HostContext.Callbacks.OnFsmComplete != nullptr);
			DISPATCHER_HandleFsmCompletion(pThread);
			CONTROLFLAGS_CLEAR_ACTIVE(pThread->ControlFlags);
			CONTROLFLAGS_SET_READY(pThread->ControlFlags);
			THREAD_TRACE("THREAD: Thread with ID %u deactivating.", pThread->ThreadId);
		}
		else
		{
			THREAD_HandleIdle(pThread);
		}
	}
	THREAD_TRACE("THREAD: Thread with ID %u exiting main loop.", pThread->ThreadId);
}

void THREAD_PrepareRoot(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const  SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(!CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags));
	ASSERT(CONTROLFLAGS_IS_ROOT(pThread->ControlFlags));
	THREAD_TRACE("THREAD: Thread with ID %u preparing root execution with DTH %u.", pThread->ThreadId, distanceToHorizon);
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->ParentId = THREADINDEX_NONE;
	pThread->NodeCount = 0;
	pThread->StateMachine = stateMachine;
	memcpy(&pThread->HostContext, pHostContext, sizeof(SE_HOSTCONTEXT));
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	NODE_Initialize(pThread, &pThread->Stack[distanceToHorizon]);
}

void THREAD_PrepareFork(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX parentId, SE_FSM stateMachine, const SE_FORK* pFork, const SE_FORKINDEX forkIndex)
{
	ASSERT(!CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags));
	ASSERT(!CONTROLFLAGS_IS_ROOT(pThread->ControlFlags));
	THREAD_TRACE("THREAD: Thread with ID %u preparing fork execution with DTH %u.", pThread->ThreadId, distanceToHorizon);
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->ParentId = parentId;
	pThread->NodeCount = 0;
	pThread->StateMachine = stateMachine;
	pThread->ActiveFork = forkIndex;
	memcpy(&pThread->HostContext, &pFork->HostContext, sizeof(SE_HOSTCONTEXT));
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	NODE_Initialize(pThread, &pThread->Stack[distanceToHorizon]);
	for (MG_MOVEINDEX forkMoveIndex = 0; forkMoveIndex < pFork->CountMoves; forkMoveIndex++)
	{
		pThread->Stack[distanceToHorizon].MoveList.Move[forkMoveIndex] = pFork->Moves[forkMoveIndex];
	}
	pThread->Stack[distanceToHorizon].MoveList.CountMoves = pFork->CountMoves;
	pThread->Stack[distanceToHorizon].State = NODESTATE_ITERATE;
}

void THREAD_Initialize(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread, const SE_THREADINDEX threadId, const MG_MOVEGEN* pMoveGen)
{
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
	THREAD_TRACE("THREAD: Thread with ID %u initializing.", threadId);
	pThread->ThreadId = threadId;
	pThread->TerminationRequested = false;
	CONTROLFLAGS_SET_INITIALIZED(pThread->ControlFlags);
	CONTROLFLAGS_SET_READY(pThread->ControlFlags);
	pThread->Handle = new std::thread([pThread, pMoveGen, pDispatcher]() { THREAD_MainLoop(pDispatcher, pThread, pMoveGen); });
	pThread->pDispatcher = pDispatcher;
	pThread->ActiveFork = FORKINDEX_NONE;
	pThread->CountIdleTicks = 0;
	pThread->RetentionState = RETENTIONSTATE_HIBERNATING;
	pThread->RetentionTransitions = 0;
	pThread->SleepCount = 0;
	pThread->YieldCount = 0;
	pThread->WakeCount = 0;
	LOCK_Initialize(&pThread->LockNodeCount);
}

void THREAD_Deinitialize(SE_THREAD* pThread)
{
	ASSERT(CONTROLFLAGS_IS_INITIALIZED(pThread->ControlFlags));
	pThread->TerminationRequested = true;
	pThread->Handle->join();
	delete pThread->Handle;
	pThread->Handle = nullptr;
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
	THREAD_TRACE("THREAD: Thread with ID %u deinitializing.", pThread->ThreadId);
}