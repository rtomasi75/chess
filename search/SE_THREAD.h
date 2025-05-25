#pragma once
#ifndef SE_THREAD_H
#define SE_THREAD_H

#include "SE_Types.h"
#include "SE_NODE.h"
#include "SE_CONTROLFLAGS.h"
#include "SE_SEARCH.h"
#include "libMovegen.h"
#include "libCommon.h"

struct CM_ALIGN_CACHELINE SE_THREAD
{
    SE_CONTROLFLAGS ControlFlags;             
    SE_POSITIONCOUNT NodeCount;               
    SE_DEPTH DistanceToHorizon;
    SE_DEPTH RootDistanceToHorizon;
    MG_POSITION SharedPosition;
    SE_NODE Stack[SEARCH_MAX_DEPTH];   
    SE_THREADINDEX ThreadId;
    SE_THREADINDEX ParentId;
};

void THREAD_InitializeRoot(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX threadId);

void THREAD_InitializeChild(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX threadId, const SE_THREADINDEX parentId);


#endif // SE_THREAD_H