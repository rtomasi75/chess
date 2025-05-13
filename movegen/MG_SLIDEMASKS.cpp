#include "MG_SLIDEMASKS.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

BB_BITBOARD SLIDEMASKS_GenerateTargetsHorizontal(const BB_SQUARE& square, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD rayTargets;
	// Up
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_UP(current);
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Down
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_DOWN(current);
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Left
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_LEFT(current);
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Right
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_RIGHT(current);
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	return targets;
}

BB_BITBOARD SLIDEMASKS_GenerateTargetsDiagonal(const BB_SQUARE& square, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD rayTargets;
	// Up/Left
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_UP(BITBOARD_LEFT(current));
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Up/Right
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_UP(BITBOARD_RIGHT(current));
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Down/Left
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_DOWN(BITBOARD_LEFT(current));
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	// Down/Right
	current = square;
	rayTargets = BITBOARD_EMPTY;
	while (current)
	{
		current = BITBOARD_DOWN(BITBOARD_RIGHT(current));
		rayTargets |= current;
		current &= ~occupancy;
	}
	targets |= rayTargets;
	return targets;
}

BB_BITBOARD SLIDEMASKS_GenerateMaskHorizontal(const BB_SQUARE& square)
{
	BB_BITBOARD mask = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD scratch;
	// Up
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_UP(current);
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(COUNT_RANKS - 1);
	mask |= scratch;
	// Down
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_DOWN(current);
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(0);
	mask |= scratch;
	// Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_RIGHT(current);
		scratch |= current;
	}
	scratch &= ~FILE_FromIndex(COUNT_FILES - 1);
	mask |= scratch;
	// Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_LEFT(current);
		scratch |= current;
	}
	scratch &= ~FILE_FromIndex(0);
	mask |= scratch;
	return mask;
}

BB_BITBOARD SLIDEMASKS_GenerateMaskDiagonal(const BB_SQUARE& square)
{
	BB_BITBOARD mask = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD scratch;
	// Up/Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_LEFT(BITBOARD_UP(current));
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(COUNT_RANKS - 1);
	scratch &= ~FILE_FromIndex(0);
	mask |= scratch;
	// Down/Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_LEFT(BITBOARD_DOWN(current));
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(0);
	scratch &= ~FILE_FromIndex(0);
	mask |= scratch;
	// Up/Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_UP(BITBOARD_RIGHT(current));
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(COUNT_RANKS - 1);
	scratch &= ~FILE_FromIndex(COUNT_FILES - 1);
	mask |= scratch;
	// Down/Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_DOWN(BITBOARD_RIGHT(current));
		scratch |= current;
	}
	scratch &= ~RANK_FromIndex(0);
	scratch &= ~FILE_FromIndex(COUNT_FILES - 1);
	mask |= scratch;
	return mask;
}

void SLIDEMASKS_InitializeDiagonal(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry)
{
	for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
	{
		const BB_SQUARE square = SQUARE_FromIndex(squareIndex);
		const BB_BITBOARD mask = SLIDEMASKS_GenerateMaskDiagonal(square);
		const BB_BITBOARD targets = SLIDEMASKS_GenerateTargetsDiagonal(square, BITBOARD_EMPTY);
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].Mask[squareIndex] = mask;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].PotentialTargets[squareIndex] = targets;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].BaseEntry[squareIndex] = nextEntry;
		const BB_SQUARECOUNT countMaskBits = BITBOARD_PopulationCount(mask);
		const BB_SQUARECOUNT countPotentialTargetsBits = BITBOARD_PopulationCount(targets);
		const MG_SLIDEENTRYINDEX countEntries = ((MG_SLIDEENTRYINDEX)1) << countMaskBits;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountEntries[squareIndex] = countEntries;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountPotentialTargetsBits[squareIndex] = countPotentialTargetsBits;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountPotentialTargets[squareIndex] = INT16_C(1) << countPotentialTargetsBits;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountMaskBits[squareIndex] = countMaskBits;
		for (MG_SLIDEENTRYINDEX idx = 0; idx < countEntries; idx++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitDeposit(idx, mask);
			const MG_SLIDEENTRYINDEX entry = nextEntry++;
			ASSERT(entry < pMoveGen->CountSlideEntries);
#ifdef MOVEGEN_COMPACT_TARGETS
			pMoveGen->SlideEntries[entry].CompressedTargets = (MG_COMPRESSEDTARGETS)BITBOARD_BitExtract(SLIDEMASKS_GenerateTargetsDiagonal(square, occupancy), targets);
#else
			pMoveGen->SlideEntries[entry].Targets = SLIDEMASKS_GenerateTargetsDiagonal(square, occupancy);
#endif
		}
	}
}

void SLIDEMASKS_InitializeHorizontal(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry)
{
	for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
	{
		const BB_SQUARE square = SQUARE_FromIndex(squareIndex);
		const BB_BITBOARD mask = SLIDEMASKS_GenerateMaskHorizontal(square);
		const BB_BITBOARD targets = SLIDEMASKS_GenerateTargetsHorizontal(square, BITBOARD_EMPTY);
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].Mask[squareIndex] = mask;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].PotentialTargets[squareIndex] = targets;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].BaseEntry[squareIndex] = nextEntry;
		const BB_SQUARECOUNT countMaskBits = BITBOARD_PopulationCount(mask);
		const BB_SQUARECOUNT countPotentialTargetsBits = BITBOARD_PopulationCount(targets);
		const MG_SLIDEENTRYINDEX countEntries = ((MG_SLIDEENTRYINDEX)1) << countMaskBits;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountEntries[squareIndex] = countEntries;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountPotentialTargetsBits[squareIndex] = countPotentialTargetsBits;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountPotentialTargets[squareIndex] = INT16_C(1) << countPotentialTargetsBits;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountMaskBits[squareIndex] = countMaskBits;
		for (MG_SLIDEENTRYINDEX idx = 0; idx < countEntries; idx++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitDeposit(idx, mask);
			const MG_SLIDEENTRYINDEX entry = nextEntry++;
			ASSERT(entry < pMoveGen->CountSlideEntries);
#ifdef MOVEGEN_COMPACT_TARGETS
			pMoveGen->SlideEntries[entry].CompressedTargets = (MG_COMPRESSEDTARGETS)BITBOARD_BitExtract(SLIDEMASKS_GenerateTargetsHorizontal(square, occupancy), targets);
#else
			pMoveGen->SlideEntries[entry].Targets = SLIDEMASKS_GenerateTargetsHorizontal(square, occupancy);
#endif
		}
	}
}

void SLIDEMASKS_Initialize(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry)
{
	SLIDEMASKS_InitializeHorizontal(pMoveGen, nextEntry);
	SLIDEMASKS_InitializeDiagonal(pMoveGen, nextEntry);
}

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntriesDiagonal()
{
	MG_SLIDEENTRYINDEX count = 0;
	for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
	{
		const BB_SQUARE square = SQUARE_FromIndex(squareIndex);
		const BB_BITBOARD mask = SLIDEMASKS_GenerateMaskDiagonal(square);
		const BB_SQUARECOUNT countMaskBits = BITBOARD_PopulationCount(mask);
		const MG_SLIDEENTRYINDEX countEntries = ((MG_SLIDEENTRYINDEX)1) << countMaskBits;
		count += countEntries;
	}
	return count;
}

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntriesHorizontal()
{
	MG_SLIDEENTRYINDEX count = 0;
	for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
	{
		const BB_SQUARE square = SQUARE_FromIndex(squareIndex);
		const BB_BITBOARD mask = SLIDEMASKS_GenerateMaskHorizontal(square);
		const BB_SQUARECOUNT countMaskBits = BITBOARD_PopulationCount(mask);
		const MG_SLIDEENTRYINDEX countEntries = ((MG_SLIDEENTRYINDEX)1) << countMaskBits;
		count += countEntries;
	}
	return count;
}

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntries()
{
	MG_SLIDEENTRYINDEX count = 0;
	count += SLIDEMASKS_CountEntriesDiagonal();
	count += SLIDEMASKS_CountEntriesHorizontal();
	return count;
}

void SLIDEMASKS_Initialize_QuietMoves(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][movingPiece];
	const MG_TABLEINDEX tableIndex = pieceInfo.TableIndex[TABLEINDEX_QUIET];
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[tableIndex].CountMasks; idx++)
		{
			pMoveGen->SlideLookUp[tableIndex].MoveBase[movingPlayer][idx][squareIndexFrom] = nextMove;
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[tableIndex].MaskIndex[idx];
			const BB_BITBOARD allTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[squareIndexFrom];
			const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(allTargets);
			ASSERT(countBits == pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom]);
			for (BB_SQUARECOUNT bitIndex = 0; bitIndex < countBits; bitIndex++)
			{
				const BB_BITBOARD bit = UINT64_C(1) << bitIndex;
				const BB_SQUARE squareTo = BITBOARD_BitDeposit(bit, allTargets);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove + bitIndex;
				ASSERT(move < pMoveGen->CountMoves[movingPlayer]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[movingPlayer][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[movingPlayer][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[movingPlayer][move].PromoMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[movingPlayer][move].MoveMap = squareFrom ^ squareTo;
#endif
				pMoveGen->MoveTable[movingPlayer][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[movingPlayer][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[movingPlayer][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[movingPlayer][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[movingPlayer][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[movingPlayer][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[movingPlayer][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[movingPlayer][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[movingPlayer][move].PromoPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[movingPlayer][move].PromoPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[movingPlayer][move].PromoSource = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[movingPlayer][move].MovePiece = movingPiece;
				pMoveGen->MoveTable[movingPlayer][move].MovePlayer = movingPlayer;
				pMoveGen->MoveTable[movingPlayer][move].EnPassantFileIndex = FILEINDEX_NONE;
				pMoveGen->MoveTable[movingPlayer][move].HashDelta = HASH_PlayerPieceSquare(movingPlayer, movingPiece, squareFrom) ^ HASH_PlayerPieceSquare(movingPlayer, movingPiece, squareTo);
				pMoveGen->MoveTable[movingPlayer][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(movingPlayer, movingPiece, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[movingPlayer][move].MoveString, squareFrom, squareTo);
			}
			nextMove += countBits;
		}
	}
}

MG_MOVE SLIDEMASKS_Count_QuietMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece)
{
	MG_MOVE count = 0;
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][movingPiece];
	const MG_TABLEINDEX tableIndex = pieceInfo.TableIndex[TABLEINDEX_QUIET];
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[tableIndex].CountMasks; idx++)
		{
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[tableIndex].MaskIndex[idx];
			count += pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom];
		}
	}
	return count;
}

void SLIDEMASKS_Initialize_CaptureMoves(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][movingPiece];
	const MG_PLAYER otherPlayer = PLAYER_OTHER(movingPlayer);
	MG_TABLEINDEX tableIndex[COUNT_PIECETYPES];
	for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
	{
		tableIndex[capturedPiece] = pieceInfo.TableIndex[TABLEINDEX_CAPTURE(capturedPiece)];
	}
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[tableIndex[capturedPiece]].CountMasks; idx++)
			{
				const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[tableIndex[capturedPiece]].MaskIndex[idx];
				const BB_BITBOARD allTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[squareIndexFrom];
				const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(allTargets);
				ASSERT(countBits == pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom]);
				pMoveGen->SlideLookUp[tableIndex[capturedPiece]].CaptureBase[movingPlayer][idx][squareIndexFrom][capturedPiece] = nextMove;
				for (BB_SQUARECOUNT bitIndex = 0; bitIndex < countBits; bitIndex++)
				{
					const BB_BITBOARD bit = UINT64_C(1) << bitIndex;
					const BB_SQUARE squareTo = BITBOARD_BitDeposit(bit, allTargets);
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
					const MG_MOVE move = nextMove + bitIndex;
					ASSERT(move < pMoveGen->CountMoves[movingPlayer]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[movingPlayer][move].KillMap = squareTo;
					pMoveGen->MoveTable[movingPlayer][move].MoveMap = squareFrom ^ squareTo;
					pMoveGen->MoveTable[movingPlayer][move].PromoMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[movingPlayer][move].CreateMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[movingPlayer][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[movingPlayer][move].MoveSource = squareIndexFrom;
					pMoveGen->MoveTable[movingPlayer][move].KillPiece = capturedPiece;
					pMoveGen->MoveTable[movingPlayer][move].KillPlayer = otherPlayer;
					pMoveGen->MoveTable[movingPlayer][move].KillDest = squareIndexTo;
					pMoveGen->MoveTable[movingPlayer][move].MovePiece = movingPiece;
					pMoveGen->MoveTable[movingPlayer][move].MovePlayer = movingPlayer;
					pMoveGen->MoveTable[movingPlayer][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[movingPlayer][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[movingPlayer][move].CreateDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[movingPlayer][move].PromoPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[movingPlayer][move].PromoPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[movingPlayer][move].PromoSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[movingPlayer][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[movingPlayer][move].HashDelta = HASH_PlayerPieceSquare(movingPlayer, movingPiece, squareFrom) ^ HASH_PlayerPieceSquare(movingPlayer, movingPiece, squareTo) ^ HASH_PlayerPieceSquare(otherPlayer, capturedPiece, squareTo);
					pMoveGen->MoveTable[movingPlayer][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(movingPlayer, movingPiece, squareFrom, squareTo, capturedPiece);
					MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[movingPlayer][move].MoveString, squareFrom, squareTo);
				}
				nextMove += countBits;
			}
		}
	}
}

MG_MOVE SLIDEMASKS_Count_CaptureMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece)
{
	MG_MOVE count = 0;
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][movingPiece];
	const MG_TABLEINDEX tableIndex = pieceInfo.TableIndex[TABLEINDEX_QUIET];
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		for (MG_SLIDEMASKINDEX idx = 0; idx < pMoveGen->SlideLookUp[tableIndex].CountMasks; idx++)
		{
			const MG_SLIDEMASKINDEX maskIndex = pMoveGen->SlideLookUp[tableIndex].MaskIndex[idx];
			count += pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[squareIndexFrom] * COUNT_PIECETYPES;
		}
	}
	return count;
}

BB_BITBOARD SLIDEMASKS_LookUpTargets(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_SLIDEMASKINDEX& maskIndex, const BB_SQUAREINDEX& fromSquareIndex)
{
	const BB_BITBOARD mask = pMoveGen->SlideMasks[maskIndex].Mask[fromSquareIndex];
	const MG_SLIDEENTRYINDEX lookUpIndex = (MG_SLIDEENTRYINDEX)CM_BitExtract(pPosition->OccupancyTotal, mask);
	const MG_SLIDEENTRYINDEX entry = pMoveGen->SlideMasks[maskIndex].BaseEntry[fromSquareIndex] + lookUpIndex;
	return SLIDEMASKS_EntryTargets(pMoveGen, entry, maskIndex, fromSquareIndex);
}

BB_BITBOARD SLIDEMASKS_EntryTargets(const MG_MOVEGEN* pMoveGen, const MG_SLIDEENTRYINDEX& entry, const MG_SLIDEMASKINDEX& maskIndex, const BB_SQUAREINDEX& fromSquareIndex)
{
	ASSERT(entry < pMoveGen->CountSlideEntries);
#ifdef MOVEGEN_COMPACT_TARGETS
	const BB_BITBOARD potTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[fromSquareIndex];
	const BB_BITBOARD targets = BITBOARD_BitDeposit(pMoveGen->SlideEntries[entry].CompressedTargets, potTargets);
	return targets;
#else
	const BB_BITBOARD targets = pMoveGen->SlideEntries[entry].Targets;
	return targets;
#endif

}

void SLIDEMASKS_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_QUIET];
	const MG_SLIDELOOKUP& table = pMoveGen->SlideLookUp[tableIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		for (MG_SLIDEMASKINDEX slideMaskIndex = 0; slideMaskIndex < table.CountMasks; slideMaskIndex++)
		{
			const MG_SLIDEMASKINDEX maskIndex = table.MaskIndex[slideMaskIndex];
			const BB_BITBOARD potentialTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[fromSquareIndex];
			const BB_BITBOARD targets = SLIDEMASKS_LookUpTargets(pMoveGen, pPosition, maskIndex, fromSquareIndex);
			BB_BITBOARD destinations = targets & ~pPosition->OccupancyTotal;
			BB_SQUAREINDEX toSquareIndex;
			while (SQUARE_Next(destinations, toSquareIndex))
			{
				const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
				const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, potentialTargets);
				const MG_MOVE move = table.MoveBase[pPosition->MovingPlayer][slideMaskIndex][fromSquareIndex] + optionIndex;
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
}

void SLIDEMASKS_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_CAPTURE(capturedPiece)];
			const MG_SLIDELOOKUP& table = pMoveGen->SlideLookUp[tableIndex];
			for (MG_SLIDEMASKINDEX slideMaskIndex = 0; slideMaskIndex < table.CountMasks; slideMaskIndex++)
			{
				const MG_SLIDEMASKINDEX maskIndex = table.MaskIndex[slideMaskIndex];
				const BB_BITBOARD targets = SLIDEMASKS_LookUpTargets(pMoveGen, pPosition, maskIndex, fromSquareIndex);
				BB_BITBOARD destinations = targets & pPosition->OccupancyPlayerPiece[pPosition->PassivePlayer][capturedPiece];
				BB_SQUAREINDEX toSquareIndex;
				while (SQUARE_Next(destinations, toSquareIndex))
				{
					const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
					const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, pMoveGen->SlideMasks[maskIndex].PotentialTargets[fromSquareIndex]);
					const MG_MOVE move = table.CaptureBase[pPosition->MovingPlayer][slideMaskIndex][fromSquareIndex][capturedPiece] + optionIndex;
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
			}
		}
	}
}

BB_BITBOARD SLIDEMASKS_GetPieceAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest)
{
	BB_BITBOARD attacks = BITBOARD_EMPTY;
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[player][piece];
	BB_SQUAREINDEX fromSquareIndex;
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[player][piece].TableIndex[TABLEINDEX_CAPTURE(piece)];
		const MG_SLIDELOOKUP& table = pMoveGen->SlideLookUp[tableIndex];
		for (MG_SLIDEMASKINDEX slideMaskIndex = 0; slideMaskIndex < table.CountMasks; slideMaskIndex++)
		{
			const MG_SLIDEMASKINDEX maskIndex = table.MaskIndex[slideMaskIndex];
			const BB_BITBOARD targets = SLIDEMASKS_LookUpTargets(pMoveGen, pPosition, maskIndex, fromSquareIndex);
			attacks |= targets;
		}
	}
	outInterest = attacks | pPosition->OccupancyPlayerPiece[player][piece];
	return attacks;
}
