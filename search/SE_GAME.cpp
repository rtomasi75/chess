#include "SE_GAME.h"

void GAME_Initialize(SE_GAME* pGame, const MG_MOVEGEN* pMoveGen)
{
	POSITION_Initialize(&pGame->CurrentPosition);
	pGame->CurrentMove = 0;
	MOVEGEN_GenerateMoves(pMoveGen, &pGame->CurrentPosition, &pGame->LegalMoves);
}

void GAME_MakeMove(SE_GAME* pGame, const MG_MOVE& move, const MG_MOVEGEN* pMoveGen)
{
	SE_PLAYEDMOVEINDEX playedMoveIndex = pGame->CurrentMove++;
	MOVEGEN_MakeMove(pMoveGen, move, &pGame->PlayedMoves[playedMoveIndex].MoveData, &pGame->CurrentPosition);
	pGame->PlayedMoves[playedMoveIndex].Move = move;
	MOVEGEN_GenerateMoves(pMoveGen, &pGame->CurrentPosition, &pGame->LegalMoves);
}

void GAME_UnmakeMove(SE_GAME* pGame, const MG_MOVEGEN* pMoveGen)
{
	SE_PLAYEDMOVEINDEX playedMoveIndex = pGame->CurrentMove--;
	MOVEGEN_UnmakeMove(pMoveGen, pGame->PlayedMoves[playedMoveIndex].Move, &pGame->PlayedMoves[playedMoveIndex].MoveData, &pGame->CurrentPosition);
	MOVEGEN_GenerateMoves(pMoveGen, &pGame->CurrentPosition, &pGame->LegalMoves);
}
