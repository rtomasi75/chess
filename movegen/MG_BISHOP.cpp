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
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_QUIET].CountMasks; idx++)
		{
			pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_QUIET].MoveBase[player][idx][squareIndexFrom] = nextMove;
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_QUIET].MaskIndex[idx];
			const BB_BITBOARD allTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[squareIndexFrom];
			const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(allTargets);
			ASSERT(countBits == pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom]);
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
				pMoveGen->MoveTable[player][move].MovePiece = PIECETYPE_BISHOP;
				pMoveGen->MoveTable[player][move].MovePlayer = player;
				MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
			}
		}
	}
}

void BISHOP_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	const MG_PLAYER otherPlayer = PLAYER_OTHER(player);
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_CAPTURE].CountMasks; idx++)
		{
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_CAPTURE].MaskIndex[idx];
			const BB_BITBOARD allTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[squareIndexFrom];
			const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(allTargets);
			ASSERT(countBits == pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom]);
			pMoveGen->SlideLookUp[SLIDELOOKUP_BISHOP_CAPTURE].MoveBase[player][idx][squareIndexFrom] = nextMove;
			for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				for (BB_SQUARECOUNT bitIndex = 0; bitIndex < countBits; bitIndex++)
				{
					const BB_BITBOARD bit = UINT64_C(1) << bitIndex;
					const BB_SQUARE squareTo = BITBOARD_BitDeposit(bit, allTargets);
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[player][move].KillMap = squareTo;
					pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
					pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
					pMoveGen->MoveTable[player][move].KillPiece = capturedPiece;
					pMoveGen->MoveTable[player][move].KillPlayer = otherPlayer;
					pMoveGen->MoveTable[player][move].KillDest = squareIndexTo;
					pMoveGen->MoveTable[player][move].MovePiece = PIECETYPE_BISHOP;
					pMoveGen->MoveTable[player][move].MovePlayer = player;
					pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
					MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
				}
			}
		}
	}
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
