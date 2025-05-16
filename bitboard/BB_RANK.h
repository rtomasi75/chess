#pragma once
#ifndef BB_RANK_H
#define BB_RANK_H
#include "BB_BITBOARD.h"
#include <cstdint>

typedef BB_BITBOARD BB_RANK;
typedef std::int8_t BB_RANKINDEX;

#define RANK_1	UINT64_C(0x00000000000000FF)
#define RANK_2	UINT64_C(0x000000000000FF00)
#define RANK_3	UINT64_C(0x0000000000FF0000)
#define RANK_4	UINT64_C(0x00000000FF000000)
#define RANK_5	UINT64_C(0x000000FF00000000)
#define RANK_6	UINT64_C(0x0000FF0000000000)
#define RANK_7	UINT64_C(0x00FF000000000000)
#define RANK_8	UINT64_C(0xFF00000000000000)

constexpr inline BB_RANK RANK_FromIndex(const BB_RANKINDEX& index)
{
	return UINT64_C(0x00000000000000FF) << (8 * index);
}

bool RANK_Parse(const char* pString, const int& len, int& strPos, BB_RANK& outParsed);

bool RANK_ToString(char* pString, const int& len, int& strPos, const BB_RANK& rank);

#define COUNT_RANKS INT8_C(8)

#endif