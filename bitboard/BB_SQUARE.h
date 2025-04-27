#pragma once
#ifndef BB_SQUARE_H
#define BB_SQUARE_H
#include "BB_RANK.h"
#include "BB_FILE.h"
#include <cstdint>

typedef BB_BITBOARD BB_SQUARE;
typedef std::int8_t BB_SQUAREINDEX;

#define SQUAREINDEX_NONE INT8_C(-1)

#define SQUARE_A1 (RANK_1&FILE_A)
#define SQUARE_A2 (RANK_2&FILE_A)
#define SQUARE_A3 (RANK_3&FILE_A)
#define SQUARE_A4 (RANK_4&FILE_A)
#define SQUARE_A5 (RANK_5&FILE_A)
#define SQUARE_A6 (RANK_6&FILE_A)
#define SQUARE_A7 (RANK_7&FILE_A)
#define SQUARE_A8 (RANK_8&FILE_A)

#define SQUARE_B1 (RANK_1&FILE_B)
#define SQUARE_B2 (RANK_2&FILE_B)
#define SQUARE_B3 (RANK_3&FILE_B)
#define SQUARE_B4 (RANK_4&FILE_B)
#define SQUARE_B5 (RANK_5&FILE_B)
#define SQUARE_B6 (RANK_6&FILE_B)
#define SQUARE_B7 (RANK_7&FILE_B)
#define SQUARE_B8 (RANK_8&FILE_B)

#define SQUARE_C1 (RANK_1&FILE_C)
#define SQUARE_C2 (RANK_2&FILE_C)
#define SQUARE_C3 (RANK_3&FILE_C)
#define SQUARE_C4 (RANK_4&FILE_C)
#define SQUARE_C5 (RANK_5&FILE_C)
#define SQUARE_C6 (RANK_6&FILE_C)
#define SQUARE_C7 (RANK_7&FILE_C)
#define SQUARE_C8 (RANK_8&FILE_C)

#define SQUARE_D1 (RANK_1&FILE_D)
#define SQUARE_D2 (RANK_2&FILE_D)
#define SQUARE_D3 (RANK_3&FILE_D)
#define SQUARE_D4 (RANK_4&FILE_D)
#define SQUARE_D5 (RANK_5&FILE_D)
#define SQUARE_D6 (RANK_6&FILE_D)
#define SQUARE_D7 (RANK_7&FILE_D)
#define SQUARE_D8 (RANK_8&FILE_D)

#define SQUARE_E1 (RANK_1&FILE_E)
#define SQUARE_E2 (RANK_2&FILE_E)
#define SQUARE_E3 (RANK_3&FILE_E)
#define SQUARE_E4 (RANK_4&FILE_E)
#define SQUARE_E5 (RANK_5&FILE_E)
#define SQUARE_E6 (RANK_6&FILE_E)
#define SQUARE_E7 (RANK_7&FILE_E)
#define SQUARE_E8 (RANK_8&FILE_E)

#define SQUARE_F1 (RANK_1&FILE_F)
#define SQUARE_F2 (RANK_2&FILE_F)
#define SQUARE_F3 (RANK_3&FILE_F)
#define SQUARE_F4 (RANK_4&FILE_F)
#define SQUARE_F5 (RANK_5&FILE_F)
#define SQUARE_F6 (RANK_6&FILE_F)
#define SQUARE_F7 (RANK_7&FILE_F)
#define SQUARE_F8 (RANK_8&FILE_F)

#define SQUARE_H1 (RANK_1&FILE_H)
#define SQUARE_H2 (RANK_2&FILE_H)
#define SQUARE_H3 (RANK_3&FILE_H)
#define SQUARE_H4 (RANK_4&FILE_H)
#define SQUARE_H5 (RANK_5&FILE_H)
#define SQUARE_H6 (RANK_6&FILE_H)
#define SQUARE_H7 (RANK_7&FILE_H)
#define SQUARE_H8 (RANK_8&FILE_H)

BB_SQUARE SQUARE_FromRankFile(const BB_RANK& rank, const BB_FILE& file);
BB_SQUARE SQUARE_FromIndex(const BB_SQUAREINDEX& index);
BB_SQUARE SQUARE_FromRankFileIndices(const BB_RANKINDEX& indexRank, const BB_FILEINDEX& indexFile);
BB_SQUAREINDEX SQUARE_GetIndex(const BB_SQUARE& square);

bool SQUARE_Next(BB_BITBOARD& bitboard, BB_SQUAREINDEX& outSquareIndex);

bool SQUARE_Parse(const char* pString, const int& len, int& strPos, BB_SQUARE& outParsed);

bool SQUARE_ToString(char* pString, const int& len, int& strPos, const BB_SQUARE& square);

BB_FILE SQUARE_GetFile(const BB_SQUARE& square);

BB_RANK SQUARE_GetRank(const BB_SQUARE& square);

#define COUNT_SQUARES (COUNT_FILES*COUNT_RANKS)

#endif