#pragma once
#ifndef MG_MOVEINFO_H
#define MG_MOVEINFO_H

#include "MG_PIECETYPE.h"
#include "MG_PLAYER.h"
#include "MG_HASH.h"
#include "MG_CASTLEFLAGS.h"
#include "libBitboard.h"

#define MOVESTRING_LENGTH 6

#define MOVEGEN_COMPACT_MOVEINFO

struct MG_MOVEINFO
{
	MG_HASH HashDelta;
#ifndef MOVEGEN_COMPACT_MOVEINFO
	BB_BITBOARD MoveMap;
	BB_BITBOARD CreateMap;
	BB_BITBOARD KillMap;
#endif
	MG_PIECETYPE MovePiece;
	MG_PLAYER MovePlayer;
	MG_PIECETYPE CreatePiece;
	MG_PLAYER CreatePlayer;
	MG_PIECETYPE KillPiece;
	MG_PLAYER KillPlayer;
	BB_SQUAREINDEX MoveSource;
	BB_SQUAREINDEX MoveDest;
	BB_SQUAREINDEX KillDest;
	BB_SQUAREINDEX CreateDest;
	MG_CASTLEFLAGS CastleRightsMask;
	char MoveString[MOVESTRING_LENGTH];
};

void MOVEINFO_InitializeMoveString(char* pMoveString, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare);

BB_BITBOARD MOVEINFO_GetMoveMap(const MG_MOVEINFO* pMoveInfo);

BB_BITBOARD MOVEINFO_GetKillMap(const MG_MOVEINFO* pMoveInfo);

BB_BITBOARD MOVEINFO_GetCreateMap(const MG_MOVEINFO* pMoveInfo);

#endif