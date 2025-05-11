#include "MG_QUEEN.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

MG_MOVE QUEEN_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer)
{
	const MG_MOVE countQuietMoves = SLIDEMASKS_Count_QuietMoves(pMoveGen, movingPlayer, PIECETYPE_QUEEN);
	const MG_MOVE countCaptureMoves = SLIDEMASKS_Count_CaptureMoves(pMoveGen, movingPlayer, PIECETYPE_QUEEN);
	const MG_MOVE count = countQuietMoves + countCaptureMoves;
	return count;
}

void QUEEN_Initialize_LookUps(MG_MOVEGEN* pMoveGen)
{
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_QUIET].CountMasks = 2;
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_QUIET].MaskIndex[0] = SLIDEMASKS_HORIZONTAL;
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_QUIET].MaskIndex[1] = SLIDEMASKS_DIAGONAL;
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_CAPTURE].CountMasks = 2;
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_CAPTURE].MaskIndex[0] = SLIDEMASKS_HORIZONTAL;
	pMoveGen->SlideLookUp[SLIDELOOKUP_QUEEN_CAPTURE].MaskIndex[1] = SLIDEMASKS_DIAGONAL;
}

void QUEEN_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_QuietMoves(player, PIECETYPE_QUEEN, pMoveGen, nextMove);
}

void QUEEN_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_CaptureMoves(player, PIECETYPE_QUEEN, pMoveGen, nextMove);
}

void QUEEN_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = false;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = SLIDELOOKUP_QUEEN_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = SLIDELOOKUP_QUEEN_CAPTURE;
	}
}
