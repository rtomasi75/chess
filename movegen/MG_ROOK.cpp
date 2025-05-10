#include "MG_ROOK.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

MG_MOVE ROOK_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	MG_MOVE count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD mask = pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].Mask[squareIndexFrom];
		std::uint64_t countIndices = (UINT64_C(1) << BITBOARD_PopulationCount(mask));
		for (std::uint64_t index = 0; index < countIndices; index++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitExtract(index, mask);
			count += BITBOARD_PopulationCount(SLIDEMASKS_QuietMovesFromSquareHorizontal(squareFrom, occupancy));
			count += BITBOARD_PopulationCount(SLIDEMASKS_CaptureMovesFromSquareHorizontal(squareFrom, occupancy)) * COUNT_PIECETYPES;
		}
	}
	return count;
}

void ROOK_Initialize_LookUps(MG_MOVEGEN* pMoveGen)
{
	pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_QUIET].CountMasks = 1;
	pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_QUIET].MaskIndex[0] = SLIDEMASKS_HORIZONTAL;
	pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_CAPTURE].CountMasks = 1;
	pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_CAPTURE].MaskIndex[0] = SLIDEMASKS_HORIZONTAL;
}

void ROOK_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_QuietMoves(player, PIECETYPE_ROOK, pMoveGen, nextMove);
}

void ROOK_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	SLIDEMASKS_Initialize_CaptureMoves(player, PIECETYPE_ROOK, pMoveGen, nextMove);
}

void ROOK_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = false;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_SLIDETABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = SLIDELOOKUP_ROOK_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = SLIDELOOKUP_ROOK_CAPTURE;
	}
}
