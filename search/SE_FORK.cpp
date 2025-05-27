#include "SE_FORK.h"

bool FORK_IsValid(const SE_FORK* pFork)
{
	return pFork->CountMoves > 0 && pFork->CountMoves <= 25 && pFork->StateMachine != nullptr;
}

void FORK_Initialize(SE_FORK* pFork, const MG_POSITION* pPosition, const MG_MOVE* pMoves, MG_MOVEINDEX countMoves, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId)
{
	ASSERT(countMoves <= SEARCH_FORK_MAX_MOVES);
	pFork->Position = *pPosition;
	pFork->CountMoves = countMoves;
	for (MG_MOVEINDEX i = 0; i < countMoves; ++i)
		pFork->Moves[i] = pMoves[i];
	pFork->DistanceToHorizon = distanceToHorizon;
	pFork->StateMachine = stateMachine;
	pFork->ParentId = parentId;
	ASSERT(FORK_IsValid(pFork));
}