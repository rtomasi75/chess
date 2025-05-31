#include "SE_FORK.h"
#include "libCommon.h"

bool FORK_IsValid(const SE_FORK* pFork)
{
	ASSERT(pFork);
	return (pFork->CountMoves <= SEARCH_FORK_MAX_MOVES) && (pFork->StateMachine != nullptr);
}

void FORK_Initialize(SE_FORK* pFork, const MG_POSITION* pPosition, SE_DEPTH distanceToHorizon, SE_FSM stateMachine, SE_THREADINDEX parentId, const SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(pFork);
	pFork->Position = *pPosition;
	pFork->CountMoves = 0;
	pFork->DistanceToHorizon = distanceToHorizon;
	pFork->StateMachine = stateMachine;
	pFork->ParentId = parentId;
	memcpy(&pFork->HostContext, pHostContext, sizeof(SE_HOSTCONTEXT));
	ASSERT(FORK_IsValid(pFork));
}

void FORK_AddMove(SE_FORK* pFork, MG_MOVE move)
{
	ASSERT(pFork != nullptr);
	ASSERT(FORK_IsValid(pFork));
	ASSERT(pFork->CountMoves < SEARCH_FORK_MAX_MOVES);
	pFork->Moves[pFork->CountMoves++] = move;
}