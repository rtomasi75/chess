#include "BB_SQUARE.h"
#include "libCommon.h"

BB_SQUARE SQUARE_FromRankFile(const BB_RANK& rank, const BB_FILE& file)
{
	return rank & file;
}

BB_SQUARE SQUARE_FromIndex(const BB_SQUAREINDEX& index)
{
	return UINT64_C(1) << index;
}

BB_SQUARE SQUARE_FromRankFileIndices(const BB_RANKINDEX& indexRank, const BB_FILEINDEX& indexFile)
{
	return SQUARE_FromRankFile(RANK_FromIndex(indexRank), FILE_FromIndex(indexFile));
}

bool SQUARE_Next(BB_BITBOARD& bitboard, BB_SQUAREINDEX& outSquareIndex)
{
	if (bitboard)
	{
		outSquareIndex = CM_BitScanForward(bitboard);
		bitboard &= ~SQUARE_FromIndex(outSquareIndex);
		return true;
	}
	else
		return false;
}

BB_SQUAREINDEX SQUARE_GetIndex(const BB_SQUARE& square)
{
	ASSERT(square);
	return CM_BitScanForward(square);
}

bool SQUARE_Parse(const char* pString, const int& len, int& strPos, BB_SQUARE& outParsed)
{
	int tmpPos = strPos;
	BB_FILE file;
	if (FILE_Parse(pString, len, tmpPos, file))
	{
		BB_RANK rank;
		if (RANK_Parse(pString, len, tmpPos, rank))
		{
			strPos = tmpPos;
			outParsed = SQUARE_FromRankFile(rank, file);
			return true;
		}
	}
	return false;
}

bool SQUARE_ToString(char* pString, const int& len, int& strPos, const BB_SQUARE& square)
{
	int tmpPos = strPos;
	BB_FILE file = SQUARE_GetFile(square);
	BB_RANK rank = SQUARE_GetRank(square);
	if (FILE_ToString(pString, len, tmpPos, file))
	{
		if (RANK_ToString(pString, len, tmpPos, rank))
		{
			strPos = tmpPos;
			return true;
		}
	}
	return false;
}

BB_FILE SQUARE_GetFile(const BB_SQUARE& square)
{
	const BB_SQUAREINDEX idx = SQUARE_GetIndex(square);
	return FILE_FromIndex(idx % COUNT_RANKS);
}

BB_RANK SQUARE_GetRank(const BB_SQUARE& square)
{
	const BB_SQUAREINDEX idx = SQUARE_GetIndex(square);
	return RANK_FromIndex(idx / COUNT_RANKS);
}

BB_RANKINDEX SQUARE_GetRankIndex(const BB_SQUAREINDEX& squareIndex)
{
	return squareIndex / COUNT_RANKS;
}

BB_FILEINDEX SQUARE_GetFileIndex(const BB_SQUAREINDEX& squareIndex)
{
	return squareIndex % COUNT_RANKS;
}
