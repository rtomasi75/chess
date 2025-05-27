#pragma once
#ifndef SE_FORK_H
#define SE_FORK_H

#include "libMovegen.h"
#include "SE_Types.h"
#include "SE_FSM.h"

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

bool FORK_IsValid(const SE_FORK* pFork);

void FORK_Initialize(SE_FORK* pFork, const MG_POSITION* pPosition, const MG_MOVE* pMoves, MG_MOVEINDEX countMoves, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId);

#endif // SE_FORK_H