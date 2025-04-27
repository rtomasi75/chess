#include "MG_SLIDEMASKS.h"
#include "MG_MOVEGEN.h"

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
	BB_BITBOARD  current;
	// Up/Left
	current = square;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_LEFT(BITBOARD_UP(current));
		mask |= current;
	}
	// Up/Right
	current = square;
	for (BB_RANKINDEX rank = 0; rank < COUNT_RANKS; rank++)
	{
		current = BITBOARD_RIGHT(BITBOARD_UP(current));
		mask |= current;
	}
	// Down/Left
	current = square;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_LEFT(BITBOARD_DOWN(current));
		mask |= current;
	}
	// Down/Right
	current = square;
	for (BB_FILEINDEX file = 0; file < COUNT_FILES; file++)
	{
		current = BITBOARD_RIGHT(BITBOARD_DOWN(current));
		mask |= current;
	}
	// Don't care about border squares
	mask &= ~FILE_FromIndex(0);
	mask &= ~FILE_FromIndex(COUNT_FILES - 1);
	mask &= ~RANK_FromIndex(0);
	mask &= ~RANK_FromIndex(COUNT_RANKS - 1);
	return mask;
}

void SLIDEMASKS_Initialize(MG_MOVEGEN* pMoveGen)
{
	for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
	{
		pMoveGen->SlideMasks[SLIDEMASKS_HORIZONTAL].Mask[squareIndex] = SLIDEMASKS_GenerateMaskHorizontal(SQUARE_FromIndex(squareIndex));
		pMoveGen->SlideMasks[SLIDEMASKS_DIAGONAL].Mask[squareIndex] = SLIDEMASKS_GenerateMaskDiagonal(SQUARE_FromIndex(squareIndex));
	}
}
