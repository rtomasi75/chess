#pragma once
#ifndef MG_MOVEINFO_H
#define MG_MOVEINFO_H

#include "MG_PIECETYPE.h"
#include "MG_PLAYER.h"
#include "MG_HASH.h"
#include "MG_CASTLEFLAGS.h"
#include "MG_MOVE.h"
#include "libBitboard.h"

#define MOVEGEN_COMPACT_MOVEINFO

struct CM_ALIGN_CACHELINE MG_MOVEINFO
{
	MG_HASH HashDelta;              // 8 bytes (high priority)

	// Piece and player identifiers (8 bytes total)
	MG_PIECETYPE MovePiece;
	MG_PLAYER MovePlayer;
	MG_PIECETYPE CreatePiece;
	MG_PLAYER CreatePlayer;
	MG_PIECETYPE KillPiece;
	MG_PLAYER KillPlayer;
	MG_PIECETYPE PromoPiece;
	MG_PLAYER PromoPlayer;

	// Square indices (5 bytes total)
	BB_SQUAREINDEX MoveSource;
	BB_SQUAREINDEX MoveDest;
	BB_SQUAREINDEX KillDest;
	BB_SQUAREINDEX CreateDest;
	BB_SQUAREINDEX PromoSource;

	// Castling, EnPassant, and flags (3 bytes total)
	MG_CASTLEFLAGS CastleRightsMask; // 1 byte
	BB_FILEINDEX EnPassantFileIndex; // 1 byte
	CM_BOOL ResetHalfMoveClock;         // 1 byte

	// Conditional bitboards (64-bit each)
#ifdef MOVEGEN_COMPACT_MOVEINFO
	CM_ALIGN_PROCWORD BB_BITBOARD MoveMap;   // 8 bytes
	CM_ALIGN_PROCWORD BB_BITBOARD CreateMap; // 8 bytes
	CM_ALIGN_PROCWORD BB_BITBOARD KillMap;   // 8 bytes
	CM_ALIGN_PROCWORD BB_BITBOARD PromoMap;  // 8 bytes
#else
#if defined(CM_ALIGNMENT_CACHELINE) && (CM_ALIGNMENT_CACHELINE >= 64)
	std::uint8_t Padding[32];
#endif
#endif
};

#define MOVESTRING_LENGTH 6

struct MG_MOVEGEN;

void MOVEINFO_InitializeMoveString(MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVE& move, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare);

void MOVEINFO_InitializeMoveStringPromotion(MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVE& move, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare, const MG_PIECETYPE& promoPiece);

inline BB_BITBOARD MOVEINFO_GetMoveMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->MoveMap;
#else
	return SQUARE_FromIndex(pMoveInfo->MoveDest) ^ SQUARE_FromIndex(pMoveInfo->MoveSource);
#endif
}

inline BB_BITBOARD MOVEINFO_GetKillMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->KillMap;
#else
	return SQUARE_FromIndex(pMoveInfo->KillDest);
#endif
}

inline BB_BITBOARD MOVEINFO_GetCreateMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->CreateMap;
#else
	return SQUARE_FromIndex(pMoveInfo->CreateDest);
#endif
}

inline BB_BITBOARD MOVEINFO_GetPromoMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->PromoMap;
#else
	return SQUARE_FromIndex(pMoveInfo->PromoSource);
#endif
}


#endif