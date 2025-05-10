#include "MG_SLIDEMASKS.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

BB_BITBOARD SLIDEMASKS_GenerateTargetsHorizontal(const BB_SQUARE& square, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD mask = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD scratch;
	// Up
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_UP(current) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Down
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_DOWN(current) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_RIGHT(current) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_LEFT(current) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	return mask;
}

BB_BITBOARD SLIDEMASKS_GenerateTargetsDiagonal(const BB_SQUARE& square, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD mask = BITBOARD_EMPTY;
	BB_BITBOARD current;
	BB_BITBOARD scratch;
	// Up/Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_LEFT(BITBOARD_UP(current)) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Down/Left
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_LEFT(BITBOARD_DOWN(current)) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Up/Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_UP(BITBOARD_RIGHT(current)) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	// Down/Right
	current = square;
	scratch = BITBOARD_EMPTY;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_DOWN(BITBOARD_RIGHT(current)) & ~occupancy;
		scratch |= current;
	}
	mask |= scratch;
	return mask;
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
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].Mask[squareIndex] = mask;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].PotentialTargets[squareIndex] = SLIDEMASKS_GenerateTargetsDiagonal(square, BITBOARD_EMPTY);
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].BaseEntry[squareIndex] = nextEntry;
		const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(mask);
		const MG_SLIDEENTRYINDEX count = ((MG_SLIDEENTRYINDEX)1) << countBits;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountEntries[squareIndex] = count;
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountPotentialTargetsBits[squareIndex] = BITBOARD_PopulationCount(pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].PotentialTargets[squareIndex]);
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountPotentialTargets[squareIndex] = INT16_C(1) << pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountPotentialTargetsBits[squareIndex];
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].CountMaskBits[squareIndex] = countBits;
		for (MG_SLIDEENTRYINDEX idx = 0; idx < count; idx++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitDeposit(idx, mask);
			const MG_SLIDEENTRYINDEX entry = nextEntry++;
			ASSERT(entry < pMoveGen->CountSlideEntries);
			pMoveGen->SlideEntries[entry].Targets = SLIDEMASKS_GenerateTargetsDiagonal(square, occupancy);
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
		const BB_SQUARECOUNT countBits = BITBOARD_PopulationCount(mask);
		const MG_SLIDEENTRYINDEX count = ((MG_SLIDEENTRYINDEX)1) << countBits;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountEntries[squareIndex] = count;
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountPotentialTargetsBits[squareIndex] = BITBOARD_PopulationCount(pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].PotentialTargets[squareIndex]);
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountPotentialTargets[squareIndex] = INT16_C(1) << pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountPotentialTargetsBits[squareIndex];
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].CountMaskBits[squareIndex] = countBits;
		for (MG_SLIDEENTRYINDEX idx = 0; idx < count; idx++)
		{
			const BB_BITBOARD occupancy = BITBOARD_BitDeposit(idx, mask);
			const MG_SLIDEENTRYINDEX entry = nextEntry++;
			ASSERT(entry < pMoveGen->CountSlideEntries);
			pMoveGen->SlideEntries[entry].Targets = SLIDEMASKS_GenerateTargetsHorizontal(square, occupancy);
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
		const std::int8_t countBits = CM_PopulationCount(mask);
		count += ((MG_SLIDEENTRYINDEX)1) << countBits;
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
		const std::int8_t countBits = CM_PopulationCount(mask);
		count += ((MG_SLIDEENTRYINDEX)1) << countBits;
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

BB_BITBOARD SLIDEMASKS_QuietMovesFromSquareHorizontal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
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

BB_BITBOARD SLIDEMASKS_QuietMovesFromSquareDiagonal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	// up/left
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_UP(BITBOARD_LEFT(current)) & ~occupancy;
		targets |= current;
	}
	// down/left
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_DOWN(BITBOARD_LEFT(current)) & ~occupancy;
		targets |= current;
	}
	// up/right
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_UP(BITBOARD_RIGHT(current)) & ~occupancy;
		targets |= current;
	}
	// down/right
	current = squareFrom;
	while (current)
	{
		current = BITBOARD_DOWN(BITBOARD_RIGHT(current)) & ~occupancy;
		targets |= current;
	}
	return targets;
}

BB_BITBOARD SLIDEMASKS_CaptureMovesFromSquareHorizontal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
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

BB_BITBOARD SLIDEMASKS_CaptureMovesFromSquareDiagonal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy)
{
	BB_BITBOARD targets = BITBOARD_EMPTY;
	BB_BITBOARD current;
	// up/left
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_UP(BITBOARD_LEFT(current));
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// down/left
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_DOWN(BITBOARD_LEFT(current));
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// up/right
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_UP(BITBOARD_RIGHT(current));
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	// down/right
	current = squareFrom;
	while (current)
	{
		const BB_BITBOARD next = BITBOARD_DOWN(BITBOARD_RIGHT(current));
		targets |= next & occupancy;
		current = next & ~occupancy;
	}
	return targets;
}
