#include "MG_BISHOP.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

MG_MOVE BISHOP_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	MG_MOVE count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD mask = pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].Mask[squareIndexFrom];
		std::uint64_t countIndices = (UINT64_C(1) << BITBOARD_PopulationCount(mask));
		for (std::uint64_t index = 0; index < countIndices; index++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitExtract(index, mask);
			count += BITBOARD_PopulationCount(SLIDEMASKS_QuietMovesFromSquareDiagonal(squareFrom, occupancy));
			count += BITBOARD_PopulationCount(SLIDEMASKS_CaptureMovesFromSquareDiagonal(squareFrom, occupancy)) * COUNT_PIECETYPES;
		}
	}
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
