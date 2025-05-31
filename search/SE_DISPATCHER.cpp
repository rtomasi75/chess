#include "SE_DISPATCHER.h"


SE_THREADINDEX DISPATCHER_DetectThreadCount()
{
#ifdef _DEBUG
	return 20;
#else
	return 20;
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
	DISPATCH_TRACE("DISPATCHER: Initialized with %u threads.", pDispatcher->CountThreads);
}

void DISPATCHER_Deinitialize(SE_DISPATCHER* pDispatcher)
{
	for (SE_THREADINDEX threadIndex = 0; threadIndex < pDispatcher->CountThreads; threadIndex++)
	{
		THREAD_Deinitialize(pDispatcher->pThreadPool + threadIndex);
	}
	delete pDispatcher->pThreadPool;
	DISPATCH_TRACE("DISPATCHER: Deinitialized.");
}

static inline bool DISPATCHER_FetchFork(SE_DISPATCHER* pDispatcher, SE_FORKINDEX& forkIndex)
{
	std::int8_t bit;
	const bool returnValue = CM_PopLsbAtomic(pDispatcher->AvailableForks, bit);
	forkIndex = (SE_FORKINDEX)bit;
	if (returnValue)
	{
		DISPATCH_TRACE("DISPATCHER: Fetched fork with ID %u.", forkIndex);
	}
	else
	{
		DISPATCH_TRACE("DISPATCHER: Fork request failed - all forks in use.", forkIndex);
	}
	return returnValue;
}

static inline void DISPATCHER_RecycleFork(SE_DISPATCHER* pDispatcher, const SE_FORKINDEX forkIndex)
{
	DISPATCH_TRACE("DISPATCHER: Recycling fork with ID %u.", forkIndex);
	pDispatcher->AvailableForks.fetch_or(UINT64_C(1) << forkIndex, std::memory_order_seq_cst);
}

bool DISPATCHER_TryFork(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, const SE_FORK* pFork, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId, const SE_FORKMASK* pForkMask, SE_FN_ONFORKSUCCESS successCallback, SE_NODE* pForkingNode)
{
	ASSERT(pDispatcher->InExecution);
	ASSERT(distanceToHorizon >= 0);
	/*	if (distanceToHorizon <= 0)
		{
			DISPATCH_TRACE("DISPATCHER: Fork request at horizon refused for thread with ID %u", parentId);
			return false;
		}*/
	LOCK_Aquire(&pDispatcher->LockThreadPool);
	DISPATCH_TRACE("DISPATCHER: Trying to fork parent thread with ID %u", parentId);
	SE_FORKINDEX forkIndex;
	if (!DISPATCHER_FetchFork(pDispatcher, forkIndex))
	{
		LOCK_Release(&pDispatcher->LockThreadPool);
		return false;
	}
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
		DISPATCH_TRACE("DISPATCHER: All thraeds busy - forking rejected.");
		DISPATCHER_RecycleFork(pDispatcher, forkIndex);
		LOCK_Release(&pDispatcher->LockThreadPool);
		return false;
	}
	memcpy(&pDispatcher->Forks[forkIndex], pFork, sizeof(SE_FORK));
	CONTROLFLAGS_CLEAR_ROOT(pTargetThread->ControlFlags);
	THREAD_PrepareFork(pTargetThread, pPosition, distanceToHorizon, parentId, stateMachine, pFork, forkIndex);
	successCallback(pForkingNode, pForkMask);
	DISPATCH_TRACE("DISPATCHER: Scheduling fork with ID %u on thread with ID %u.", forkIndex, pTargetThread->ThreadId);
	CONTROLFLAGS_CLEAR_READY(pTargetThread->ControlFlags);
	CONTROLFLAGS_SET_ACTIVE(pTargetThread->ControlFlags);
	LOCK_Release(&pDispatcher->LockThreadPool);
	return true;
}

void DISPATCHER_Dispatch(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(!pDispatcher->InExecution);
	LOCK_Aquire(&pDispatcher->LockThreadPool);
	pDispatcher->InExecution = true;
	for (SE_THREADINDEX threadIndex = 0; threadIndex < pDispatcher->CountThreads; threadIndex++)
	{
		THREAD_WakeUp(pDispatcher->pThreadPool + threadIndex);
	}
	SE_THREAD* pTargetThread = &pDispatcher->pThreadPool[0];
	ASSERT(CONTROLFLAGS_IS_READY(pTargetThread->ControlFlags));
	CONTROLFLAGS_CLEAR_READY(pTargetThread->ControlFlags);
	CONTROLFLAGS_SET_ROOT(pTargetThread->ControlFlags);
	THREAD_PrepareRoot(pTargetThread, pPosition, distanceToHorizon, stateMachine, pHostContext);
	DISPATCH_TRACE("DISPATCHER: Scheduling root FSM on thread with ID %u.", pTargetThread->ThreadId);
	CONTROLFLAGS_SET_ACTIVE(pTargetThread->ControlFlags);
	LOCK_Release(&pDispatcher->LockThreadPool);
}

void DISPATCHER_HandleFsmCompletion(SE_THREAD* pThread)
{
	if (pThread->ThreadId == 0)
	{
		for (SE_THREADINDEX threadIndex = 0; threadIndex < pThread->pDispatcher->CountThreads; threadIndex++)
		{
			THREAD_Hibernate(pThread->pDispatcher->pThreadPool + threadIndex);
		}
		pThread->pDispatcher->InExecution = false;
	}
	DISPATCH_TRACE("DISPATCHER: Thread with ID %u has signaled completion.", pThread->ThreadId);
	pThread->HostContext.Callbacks.OnFsmComplete(pThread->HostContext.Token, &pThread->HostContext.SearchContext, pThread);
}

void DISPATCHER_SignalRunning(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread)
{
	ASSERT(CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags));
	ASSERT(CONTROLFLAGS_IS_RUNNING(pThread->ControlFlags));
	DISPATCH_TRACE("DISPATCHER: Thread with ID %u has signaled that it is running.", pThread->ThreadId);
	if (pThread->ThreadId != 0)
	{
		ASSERT(pThread->ActiveFork != FORKINDEX_NONE);
		DISPATCHER_RecycleFork(pDispatcher, pThread->ActiveFork);
		pThread->ActiveFork = FORKINDEX_NONE;
	}
}