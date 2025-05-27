#include "SE_THREAD.h"
#include "SE_DISPATCHER.h"
#include <cstring>

static void THREAD_MainLoop(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread, const MG_MOVEGEN* pMoveGen)
{
	while (!pThread->TerminationRequested)
	{
		if (CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags))
		{
			ASSERT(pThread->StateMachine != nullptr);
			pThread->StateMachine(pThread, pThread->pHostContext->pSearchContext, pMoveGen);
			CONTROLFLAGS_CLEAR_ACTIVE(pThread->ControlFlags);
			CONTROLFLAGS_SET_READY(pThread->ControlFlags);
			ASSERT(pThread->pHostContext != nullptr);
			ASSERT(pThread->pHostContext->pCallbacks != nullptr);
			ASSERT(pThread->pHostContext->pCallbacks->OnFsmComplete != nullptr);
			if (pThread->ThreadId == 0)
			{
				pDispatcher->InExecution = false;
				pThread->pHostContext->pCallbacks->OnFsmComplete(pThread->pHostContext->Token);
			}
		}
		std::this_thread::yield();
	}
}

void THREAD_PrepareRoot(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const  SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(!CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags));
	ASSERT(CONTROLFLAGS_IS_ROOT(pThread->ControlFlags));
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->ParentId = THREADINDEX_NONE;
	pThread->NodeCount = 0;
	pThread->StateMachine = stateMachine;
	pThread->pHostContext = pHostContext;
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	NODE_Initialize(&pThread->Stack[distanceToHorizon]);
}

void THREAD_PrepareFork(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX parentId, SE_FSM stateMachine, const  SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(!CONTROLFLAGS_IS_ACTIVE(pThread->ControlFlags));
	ASSERT(!CONTROLFLAGS_IS_ROOT(pThread->ControlFlags));
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->ParentId = parentId;
	pThread->NodeCount = 0;
	pThread->StateMachine = stateMachine;
	pThread->pHostContext = pHostContext;
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	NODE_Initialize(&pThread->Stack[distanceToHorizon]);
}

void THREAD_Initialize(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread, const SE_THREADINDEX threadId, const MG_MOVEGEN* pMoveGen)
{
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
	pThread->ThreadId = threadId;
	pThread->TerminationRequested = false;
	CONTROLFLAGS_SET_INITIALIZED(pThread->ControlFlags);
	CONTROLFLAGS_SET_READY(pThread->ControlFlags);
	pThread->Handle = new std::thread([pThread, pMoveGen, pDispatcher]() { THREAD_MainLoop(pDispatcher, pThread, pMoveGen); });
}

void THREAD_Deinitialize(SE_THREAD* pThread)
{
	ASSERT(CONTROLFLAGS_IS_INITIALIZED(pThread->ControlFlags));
	pThread->TerminationRequested = true;
	pThread->Handle->join();
	delete pThread->Handle;
	pThread->Handle = nullptr;
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
}