#pragma once
#ifndef SE_THREAD_H
#define SE_THREAD_H

#include "SE_Types.h"
#include "SE_NODE.h"
#include "SE_CONTROLFLAGS.h"
#include "SE_SEARCH.h"
#include "SE_FORK.h"
#include "libMovegen.h"
#include "libCommon.h"

#include <thread>
#include <atomic>

#include "SE_HOSTCONTEXT.h"

typedef std::thread* SE_SYSTEMTHREADHANDLE;
typedef std::atomic_bool SE_TERMINATIONFLAG;

struct SE_DISPATCHER; 

struct SE_THREAD
{
	SE_ATOMICCONTROLFLAGS ControlFlags;
	SE_POSITIONCOUNT NodeCount;
	SE_DEPTH DistanceToHorizon;
	SE_DEPTH RootDistanceToHorizon;
	MG_POSITION SharedPosition;
	SE_NODE Stack[SEARCH_MAX_DEPTH];
	SE_THREADINDEX ThreadId;
	SE_THREADINDEX ParentId;
	SE_SYSTEMTHREADHANDLE Handle;
	SE_TERMINATIONFLAG TerminationRequested;
	SE_FSM StateMachine;
	SE_DISPATCHER* pDispatcher;
	SE_HOSTCONTEXT HostContext;
	CM_LOCK LockNodeCount; 
	SE_FORKINDEX ActiveFork;
	SE_ATOMICRETENTIONSTATE RetentionState;
	SE_TICKCOUNT CountIdleTicks;
	CM_ATOMICCOUNTER SleepCount;
	CM_ATOMICCOUNTER YieldCount;
	CM_ATOMICCOUNTER WakeCount;
	CM_ATOMICCOUNTER RetentionTransitions;
};

struct SE_DISPATCHER;

void THREAD_PrepareRoot(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const  SE_HOSTCONTEXT* pHostContext);

void THREAD_PrepareFork(SE_THREAD* pThread, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, const SE_THREADINDEX parentId, SE_FSM stateMachine, const SE_FORK* pFork, const SE_FORKINDEX forkIndex);

void THREAD_Initialize(SE_DISPATCHER* pDispatcher, SE_THREAD* pThread, const SE_THREADINDEX threadId, const MG_MOVEGEN* pMoveGen);

void THREAD_Deinitialize(SE_THREAD* pThread);

void THREAD_WakeUp(SE_THREAD* pThread);

void THREAD_Hibernate(SE_THREAD* pThread);

#ifdef SEARCH_TRACE_THREAD
#define THREAD_TRACE(fmt, ...) CM_TRACE(fmt, ##__VA_ARGS__)
#else
#define THREAD_TRACE(fmt, ...) do { } while (0)
#endif

#endif // SE_THREAD_H