#include "BB_BITBOARD.h"
#include "BB_FILE.h"
#include "BB_RANK.h"
#include "BB_SQUARE.h"
#include "libCommon.h"


BB_SQUARECOUNT BITBOARD_PopulationCount(const BB_BITBOARD& bitboard)
{
	return (BB_SQUAREINDEX)CM_PopulationCount(bitboard);
}


BB_BITBOARD BITBOARD_BitDeposit(const BB_BITBOARD& bitboard, const BB_BITBOARD& mask)
{
	return CM_BitDeposit(bitboard, mask);
}

BB_BITBOARD BITBOARD_BitExtract(const BB_BITBOARD& bitboard, const BB_BITBOARD& mask)
{
	return CM_BitExtract(bitboard, mask);
}
