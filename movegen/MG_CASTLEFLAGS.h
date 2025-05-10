#pragma once
#ifndef MG_CASTLEFLAGS_H
#define MG_CASTLEFLAGS_H

#include <cstdint>

typedef std::uint8_t MG_CASTLEFLAGS;

#define CASTLEFLAGS_NONE UINT8_C(0)
#define CASTLEFLAGS_WHITE_KINGSIDE UINT8_C(1)
#define CASTLEFLAGS_WHITE_QUEENSIDE UINT8_C(2)
#define CASTLEFLAGS_BLACK_KINGSIDE UINT8_C(4)
#define CASTLEFLAGS_BLACK_QUEENSIDE UINT8_C(8)

bool CASTLEFLAGS_Parse(const char* pString, const int& len, int& strPos, MG_CASTLEFLAGS& outCastleFlags);

bool CASTLEFLAGS_ToString(char* pString, const int& len, int& strPos, const MG_CASTLEFLAGS& castleFlags);


#endif