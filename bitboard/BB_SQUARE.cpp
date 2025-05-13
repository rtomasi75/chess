#include "BB_SQUARE.h"

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
