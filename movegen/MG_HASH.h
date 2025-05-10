#pragma once
#ifndef MG_HASH_H
#define MG_HASH_H

#include <stdint.h>
#include "MG_CASTLEFLAGS.h"
#include "MG_PLAYER.h"
#include "MG_PIECETYPE.h"
#include "libBitboard.h"

typedef std::uint64_t MG_HASH;

#define HASH_EMPTY UINT64_C(0)

#define HASH_MOVINGPLAYER_BLACK UINT64_C(0x1B2699D5006885DF)

MG_HASH HASH_CastleRights(const MG_CASTLEFLAGS& castlingRights);

MG_HASH HASH_PlayerPieceSquare(const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& square);

#endif // MG_HASH_H
