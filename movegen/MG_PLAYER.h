#pragma once
#ifndef MG_PLAYER_H
#define MG_PLAYER_H

#include <cstdint>

typedef std::int8_t MG_PLAYER;

#define COUNT_PLAYERS 2

#define PLAYER_NONE INT8_C(-1)
#define PLAYER_WHITE INT8_C(0)
#define PLAYER_BLACK INT8_C(1)

#define PLAYER_OTHER(_PL_) ((MG_PLAYER)(((_PL_)+1)%COUNT_PLAYERS))

bool PLAYER_Parse(const char* pString, const int& len, int& strPos, MG_PLAYER& outParsedPlayer);

bool PLAYER_ToString(char* pString, const int& len, int& strPos, const MG_PLAYER& player);

#endif