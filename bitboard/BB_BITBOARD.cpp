#include "BB_BITBOARD.h"
#include "BB_FILE.h"
#include "BB_RANK.h"
#include "BB_SQUARE.h"
#include <iostream>

void BITBOARD_Dump(const BB_BITBOARD& bitboard, const char* pStr)
{
	if (pStr)
		std::cout << pStr << std::endl;
	for (BB_RANKINDEX rankIndex = COUNT_RANKS - 1; rankIndex >= 0; rankIndex--)
	{
		for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
		{
			BB_SQUARE square = SQUARE_FromRankFileIndices(rankIndex, fileIndex);
			if (bitboard & square)
				std::cout << "#";
			else
				std::cout << ".";
		}
		std::cout << std::endl;
	}

}