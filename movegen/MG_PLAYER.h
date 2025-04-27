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

#endif