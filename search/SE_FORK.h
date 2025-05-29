#pragma once
#ifndef SE_FORK_H
#define SE_FORK_H

#include "libMovegen.h"
#include "SE_Types.h"
#include "SE_FSM.h"
#include "SE_FORKMASK.h"

#define SEARCH_FORK_MAX_MOVES 25

struct CM_ALIGN_CACHELINE SE_FORK
{
	MG_POSITION Position;
	MG_MOVEINDEX CountMoves;
	MG_MOVE Moves[SEARCH_FORK_MAX_MOVES];
	SE_DEPTH DistanceToHorizon;
	SE_FSM StateMachine;
	SE_THREADINDEX ParentId;
	std::uint8_t Padding;
};

static_assert((sizeof(SE_FORK) - sizeof(MG_POSITION)) == 128);

bool FORK_IsValid(const SE_FORK* pFork);

void FORK_Initialize(SE_FORK* pFork, const MG_POSITION* pPosition,SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId);

void FORK_AddMove(SE_FORK* pFork, MG_MOVE move);

#endif // SE_FORK_H