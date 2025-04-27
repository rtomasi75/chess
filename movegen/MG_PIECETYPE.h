#pragma once
#ifndef MG_PIECETYPE_H
#define MG_PIECETYPE_H

#include <cstdint>
#include "MG_PLAYER.h"

typedef std::int8_t MG_PIECETYPE;

#define PIECETYPE_NONE INT8_C(-1)
#define PIECETYPE_KING INT8_C(0)
#define PIECETYPE_ROOK INT8_C(1)

#define COUNT_PIECETYPES 2


bool PIECETYPE_Parse(const char* pString, const int& len, int& strPos, MG_PIECETYPE& outParsedPiece, MG_PLAYER& outParsedPlayer);

bool PIECETYPE_ToString(char* pString, const int& len, int& strPos, const MG_PIECETYPE& piece, const MG_PLAYER& player);

#endif