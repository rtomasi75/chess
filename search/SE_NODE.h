#pragma once
#ifndef SE_NODE_H
#define SE_NODE_H

#include "libMovegen.h"
#include "SE_Types.h"
#include "SE_FORKMASK.h"
#include <atomic>

typedef std::atomic<std::uint8_t> SE_FORKCOUNT;

struct SE_THREAD;

struct SE_NODE
{
	MG_MOVEINDEX MoveIndex;
	MG_MOVELIST MoveList;
	MG_MOVEDATA MoveData;
	SE_NODEFLAGS Flags;
	SE_NODESTATE State;
	SE_FORKMASK ForkMask; 
	SE_FORKCOUNT CountLiveForks;
	SE_THREAD* pThread;
};

void NODE_Initialize(SE_THREAD* pThread, SE_NODE* pNode);

void NODE_GenerateMoves(SE_NODE* pNode, MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen);

#endif