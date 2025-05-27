#pragma once
#ifndef SE_DISPATCHER_H
#define SE_DISPATCHER_H

#include "SE_Types.h"
#include "SE_THREAD.h"
#include "SE_FORK.h"

#include <cstdint>
#include <atomic>

#define SEARCH_MAX_FORKS 64

typedef std::atomic<std::uint64_t> SE_FORKMASK;

typedef std::uint8_t SE_FORKINDEX;

#define FORKMASK_ALL UINT64_C(0xffffffffffffffff)

struct SE_DISPATCHER
{
	SE_THREADINDEX CountThreads;
	SE_THREAD* pThreadPool;
	SE_FORK Forks[SEARCH_MAX_FORKS];
	SE_FORKMASK AvailableForks;
	CM_ATOMICBOOL InExecution;
};

void DISPATCHER_Initialize(SE_DISPATCHER* pDispatcher, const MG_MOVEGEN* pMoveGen);

void DISPATCHER_Deinitialize(SE_DISPATCHER* pDispatcher);

SE_THREADINDEX DISPATCHER_DetectThreadCount();

SE_THREADINDEX DISPATCHER_GetThreadCount();

bool DISPATCHER_TryFork(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, const MG_MOVELIST* pMoveList, const MG_MOVEINDEX moveIndex, const MG_MOVEINDEX countMoves, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId, const SE_HOSTCONTEXT* pHostContext);

void DISPATCHER_Dispatch(SE_DISPATCHER* pDispatcher, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, const SE_HOSTCONTEXT* pHostContext);

#endif // SE_DISPTACHER_H