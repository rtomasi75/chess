#include "SE_THREAD.h"
#include "SE_DISPATCHER.h"
#include <cstring>

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
			CONTROLFLAGS_SET_RUNNING(pThread->ControlFlags);
			DISPATCHER_SignalRunning(pDispatcher, pThread);
			pThread->StateMachine(pThread, pMoveGen);
			ASSERT(pThread->HostContext.Callbacks.OnFsmComplete != nullptr);
			DISPATCHER_HandleFsmCompletion(pThread);
			CONTROLFLAGS_CLEAR_ACTIVE(pThread->ControlFlags);
			CONTROLFLAGS_SET_READY(pThread->ControlFlags);
			THREAD_TRACE("THREAD: Thread with ID %u deactivating.", pThread->ThreadId);
		}
		std::this_thread::yield();
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

void THREAD_PrepareFork(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX parentId, SE_FSM stateMachine, const  SE_HOSTCONTEXT* pHostContext, const SE_FORK* pFork, const SE_FORKINDEX forkIndex)
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
	memcpy(&pThread->HostContext, pHostContext, sizeof(SE_HOSTCONTEXT));
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