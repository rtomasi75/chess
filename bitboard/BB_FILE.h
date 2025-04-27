#pragma once
#ifndef BB_FILE_H
#define BB_FILE_H
#include "BB_BITBOARD.h"
#include <cstdint>

typedef BB_BITBOARD BB_FILE;
typedef std::int8_t BB_FILEINDEX;

#define FILE_A	UINT64_C(0x0101010101010101)
#define FILE_B	UINT64_C(0x0202020202020202)
#define FILE_C	UINT64_C(0x0404040404040404)
#define FILE_D	UINT64_C(0x0808080808080808)
#define FILE_E	UINT64_C(0x1010101010101010)
#define FILE_F	UINT64_C(0x2020202020202020)
#define FILE_G	UINT64_C(0x4040404040404040)
#define FILE_H	UINT64_C(0x8080808080808080)

BB_FILE FILE_FromIndex(const BB_FILEINDEX& index);

bool FILE_Parse(const char* pString, const int& len, int& strPos, BB_FILE& outParsed);

bool FILE_ToString(char* pString, const int& len, int& strPos, const BB_FILE& file);

#define COUNT_FILES INT16_C(8)

#endif