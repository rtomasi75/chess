#include "MG_ROOK.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

size_t ROOK_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	size_t count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD mask = pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].Mask[squareIndexFrom];
		BB_SQUARECOUNT countIndices = (BB_SQUARECOUNT)(UINT64_C(1) << BITBOARD_PopulationCount(mask));
		for (BB_SQUARECOUNT index = 0; index < countIndices; index++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitExtract(index, mask);
			count += BITBOARD_PopulationCount(ROOK_QuietMovesFromSquare(squareFrom, occupancy));
			count += BITBOARD_PopulationCount(ROOK_CaptureMovesFromSquare(squareFrom, occupancy)) * COUNT_PIECETYPES;
		}
	}
	return count;
}

BB_BITBOARD ROOK_QuietMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	// up
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_UP(current) & ~occupancy;
		targets |= current;
	}
	// down
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_DOWN(current) & ~occupancy;
		targets |= current;
	}
	// left
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_LEFT(current) & ~occupancy;
		targets |= current;
	}
	// right
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_RIGHT(current) & ~occupancy;
		targets |= current;
	}
	return targets;
}

BB_BITBOARD ROOK_CaptureMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	// up
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_UP(current);
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// down
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_DOWN(current);
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// left
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_LEFT(current);
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// right
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_RIGHT(current);
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	return targets;
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
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_QUIET].CountMasks; idx++)
		{
			pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_QUIET].MoveBase[player][idx][squareIndexFrom] = nextMove;
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[SLIDELOOKUP_ROOK_QUIET].MaskIndex[idx];
			const BB_BITBOARD allTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[squareIndexFrom];
			const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(allTargets);
			for (BB_SQUARECOUNT bitIndex = 0; bitIndex < countBits; bitIndex++)
			{
				const BB_BITBOARD bit = UINT64_C(1) << bitIndex;
				const BB_SQUARE squareTo = BITBOARD_BitDeposit(bit, allTargets);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[player][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
#endif
				pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[player][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[player][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[player][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[player][move].MovePiece = PIECETYPE_ROOK;
				pMoveGen->MoveTable[player][move].MovePlayer = player;
				MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
			}
		}
	}
}

void ROOK_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	/*	const MG_PLAYER otherPlayer = PLAYER_OTHER(player);
		for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
		{
			const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
			const BB_BITBOARD mask = ROOK_GenerateMask(squareFrom);
			SLIDELOOKUP_Initialize(&pSlideTable->LookUpFrom[squareIndexFrom], mask);
			const BB_BITBOARD allTargets = ROOK_QuietMovesFromSquare(squareFrom, BITBOARD_EMPTY);
			BB_SQUARECOUNT count = BITBOARD_PopulationCount(allTargets);
			for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				pSlideTable->LookUpFrom[squareIndexFrom].CaptureMoveBase[capturedPiece] = nextMove;
				for (BB_SQUARECOUNT squareBit = 0; squareBit < count; squareBit++)
				{
					BB_BITBOARD squareBitIndex = UINT64_C(1) << squareBit;
					const BB_SQUARE squareTo = BITBOARD_BitDeposit(squareBitIndex, allTargets);
					const MG_MOVE move = nextMove++;
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
	#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveInfo[move].KillMap = squareTo;
					pMoveInfo[move].MoveMap = squareFrom ^ squareTo;
					pMoveInfo[move].CreateMap = BITBOARD_EMPTY;
	#endif
					pMoveInfo[move].MoveDest = squareIndexTo;
					pMoveInfo[move].MoveSource = squareIndexFrom;
					pMoveInfo[move].KillPiece = capturedPiece;
					pMoveInfo[move].KillPlayer = otherPlayer;
					pMoveInfo[move].KillDest = squareIndexTo;
					pMoveInfo[move].MovePiece = PIECETYPE_ROOK;
					pMoveInfo[move].MovePlayer = player;
					pMoveInfo[move].CreatePiece = PIECETYPE_NONE;
					pMoveInfo[move].CreatePlayer = PLAYER_NONE;
					pMoveInfo[move].CreateDest = SQUAREINDEX_NONE;
					MOVEINFO_InitializeMoveString(pMoveInfo[move].MoveString, squareFrom, squareTo);
				}
			}
			for (MG_SLIDELOOKUPINDEX index = 0; index < pSlideTable->LookUpFrom[squareIndexFrom].TableSize; index++)
			{
				const BB_BITBOARD occupancy = BITBOARD_BitDeposit(index, mask);
				const BB_BITBOARD targets = ROOK_CaptureMovesFromSquare(squareFrom, occupancy);
				pSlideTable->LookUpFrom[squareIndexFrom].Table[index].Targets = targets;
			}
		}*/

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
