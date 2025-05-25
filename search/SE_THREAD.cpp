#include "SE_THREAD.h"

void THREAD_InitializeRoot(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX threadId)
{
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->NodeCount = 0;
	pThread->ThreadId = threadId;
	pThread->ParentId = THREADINDEX_NONE;
	CONTROLFLAGS_SET_ROOT(pThread->ControlFlags);
	for (size_t distanceToHorizon = 0; distanceToHorizon < SEARCH_MAX_DEPTH; distanceToHorizon++)
	{
		NODE_Initialize(pThread->Stack + distanceToHorizon);
	}
	CONTROLFLAGS_SET_INITIALIZED(pThread->ControlFlags);
}

void THREAD_InitializeChild(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX threadId, const SE_THREADINDEX parentId)
{
	CONTROLFLAGS_CLEAR(pThread->ControlFlags);
	memcpy(&pThread->SharedPosition, pPosition, sizeof(MG_POSITION));
	pThread->DistanceToHorizon = distanceToHorizon;
	pThread->RootDistanceToHorizon = distanceToHorizon;
	pThread->NodeCount = 0;
	pThread->ThreadId = threadId;
	pThread->ParentId = parentId;
	for (size_t stackIndex = 0; stackIndex < SEARCH_MAX_DEPTH; stackIndex++)
	{
		NODE_Initialize(pThread->Stack + distanceToHorizon);
	}
	CONTROLFLAGS_SET_INITIALIZED(pThread->ControlFlags);
}
