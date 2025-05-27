#include "SE_DISPATCHER.h"


SE_THREADINDEX DISPATCHER_DetectThreadCount()
{
#ifdef _DEBUG
	return 2;
#else
	return 1;
#endif
}

SE_THREADINDEX DISPATCHER_GetThreadCount()
{
	return DISPATCHER_DetectThreadCount();
}

void DISPATCHER_Initialize(SE_DISPATCHER* pDispatcher, const MG_MOVEGEN* pMoveGen)
{
	pDispatcher->CountThreads = DISPATCHER_GetThreadCount();
	pDispatcher->pThreadPool = new SE_THREAD[pDispatcher->CountThreads];
	pDispatcher->AvailableForks = FORKMASK_ALL;
	for (SE_THREADINDEX threadIndex = 0; threadIndex < pDispatcher->CountThreads; threadIndex++)
	{
		THREAD_Initialize(pDispatcher, pDispatcher->pThreadPool + threadIndex, threadIndex, pMoveGen);
	}
	pDispatcher->InExecution = false;
}

void DISPATCHER_Deinitialize(SE_DISPATCHER* pDispatcher)
{
	for (SE_THREADINDEX threadIndex = 0; threadIndex < pDispatcher->CountThreads; threadIndex++)
	{
		THREAD_Deinitialize(pDispatcher->pThreadPool + threadIndex);
	}
	delete pDispatcher->pThreadPool;
}

static inline bool DISPATCHER_FetchFork(SE_DISPATCHER* pDispatcher, SE_FORKINDEX& forkIndex)
{
	std::int8_t bit;
	const bool returnValue = CM_PopLsbAtomic(pDispatcher->AvailableForks, bit);
	forkIndex = (SE_FORKINDEX)bit;
	return returnValue;
}

static inline void DISPATCHER_RecycleFork(SE_DISPATCHER* pDispatcher, const SE_FORKINDEX forkIndex)
{
	pDispatcher->AvailableForks.fetch_or(UINT64_C(1) << forkIndex, std::memory_order_release);
}

bool DISPATCHER_TryFork(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, const MG_MOVELIST* pMoveList, const MG_MOVEINDEX moveIndex, const MG_MOVEINDEX countMoves, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId, const SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(pDispatcher->InExecution);
	SE_FORKINDEX forkIndex;
	if (!DISPATCHER_FetchFork(pDispatcher, forkIndex))
		return false;
	SE_THREAD* pTargetThread = nullptr;
	for (SE_THREADINDEX threadIndex = 0; threadIndex < pDispatcher->CountThreads; threadIndex++)
	{
		SE_THREAD* pCandidateThread = &pDispatcher->pThreadPool[threadIndex];
		if (CONTROLFLAGS_IS_READY(pCandidateThread->ControlFlags))
		{
			pTargetThread = pCandidateThread;
			break;
		}
	}
	if (!pTargetThread)
	{
		DISPATCHER_RecycleFork(pDispatcher, forkIndex);
		return false;
	}
	SE_FORK* pFork = &pDispatcher->Forks[forkIndex];
	FORK_Initialize(pFork, pPosition, pMoveList->Move + moveIndex, countMoves, distanceToHorizon, stateMachine, parentId);
	CONTROLFLAGS_CLEAR_READY(pTargetThread->ControlFlags);
	CONTROLFLAGS_CLEAR_ROOT(pTargetThread->ControlFlags);
	THREAD_PrepareFork(pTargetThread, pPosition, distanceToHorizon, parentId, stateMachine, pHostContext);
	CONTROLFLAGS_SET_ACTIVE(pTargetThread->ControlFlags);
	DISPATCHER_RecycleFork(pDispatcher, forkIndex);
	return true;
}

void DISPATCHER_Dispatch(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(!pDispatcher->InExecution);
	pDispatcher->InExecution = true;
	SE_THREAD* pTargetThread = &pDispatcher->pThreadPool[0];
	ASSERT(CONTROLFLAGS_IS_READY(pTargetThread->ControlFlags));
	CONTROLFLAGS_CLEAR_READY(pTargetThread->ControlFlags);
	CONTROLFLAGS_SET_ROOT(pTargetThread->ControlFlags);
	THREAD_PrepareRoot(pTargetThread, pPosition, distanceToHorizon, stateMachine, pHostContext);
	CONTROLFLAGS_SET_ACTIVE(pTargetThread->ControlFlags);
}