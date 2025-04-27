#pragma once
#ifndef MG_MOVE_H
#define MG_MOVE_H

#include <cstdint>
#include "libBitboard.h"
#include "MG_MOVETYPE.h"
#include "MG_PIECETYPE.h"
#include "MG_PLAYER.h"
#include "MG_MOVEINFO.h"

typedef std::uint16_t MG_MOVE;

#define MOVE_NULLMOVE UINT16_C(0)

MG_MOVE MOVE(const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& from, const BB_SQUARE& to, const MG_MOVETYPE& moveType);

void MOVE_InitializeNullMove(MG_MOVEINFO* pMoveInfo, const MG_PLAYER& movingPlayer);

#endif