#include "MG_BISHOP.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

MG_MOVE BISHOP_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer)
{
	const MG_MOVE countQuietMoves = SLIDEMASKS_Count_QuietMoves(pMoveGen, movingPlayer, PIECETYPE_BISHOP);
	const MG_MOVE countCaptureMoves = SLIDEMASKS_Count_CaptureMoves(pMoveGen, movingPlayer, PIECETYPE_BISHOP);
	const MG_MOVE count = countQuietMoves + countCaptureMoves;
	return count;
}

void BISHOP_Initialize_LookUps(MG_MOVEGEN* pMoveGen)
{
	pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_QUIET].CountMasks = 1;
	pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_QUIET].MaskIndex[0] = SLIDEMASKS_DIAGONAL;
	pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_CAPTURE].CountMasks = 1;
	pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_CAPTURE].MaskIndex[0] = SLIDEMASKS_DIAGONAL;
}

void BISHOP_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_QuietMoves(player, PIECETYPE_BISHOP, pMoveGen, nextMove);
}

void BISHOP_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_CaptureMoves(player, PIECETYPE_BISHOP, pMoveGen, nextMove);
}

void BISHOP_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = false;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = SLIDELOOKUP_BISHOP_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = SLIDELOOKUP_BISHOP_CAPTURE;
	}
}
