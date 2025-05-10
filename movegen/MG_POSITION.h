#pragma once
#ifndef MG_POSITION_H
#define MG_POSITION_H

#include "libBitboard.h"
#include "MG_PLAYER.h"
#include "MG_PIECETYPE.h"
#include "MG_CASTLEFLAGS.h"
#include "MG_HASH.h"

struct MG_POSITION
{
	BB_BITBOARD OccupancyPlayer[COUNT_PLAYERS];
	BB_BITBOARD OccupancyPlayerPiece[COUNT_PLAYERS][COUNT_PIECETYPES];
	BB_BITBOARD OccupancyTotal;
	MG_HASH Hash;
	MG_PLAYER MovingPlayer;
	MG_PLAYER PassivePlayer;
	MG_CASTLEFLAGS CastlingRights;
};

void POSITION_Clear(MG_POSITION* pPosition);

void POSITION_SetPiece(MG_POSITION* pPosition, const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& square);

void POSITION_Initialize(MG_POSITION* pPosition);

bool POSITION_GetPiece(const MG_POSITION* pPosition, const BB_SQUARE& square, MG_PLAYER& outPlayer, MG_PIECETYPE& outPiece);

bool POSITION_CheckConsistency(const MG_POSITION* pPosition, const BB_SQUARE& square);

#endif