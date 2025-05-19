#pragma once
#ifndef MG_JUMPTABLE_H
#define MG_JUMPTABLE_H

#include "libBitboard.h"
#include "MG_MOVE.h"

typedef std::int16_t MG_JUMPTARGETSINDEX;

struct MG_MOVEGEN;

struct MG_MOVELIST;

struct MG_POSITION;

struct CM_ALIGN_CACHELINE MG_JUMPTABLE
{
	MG_MOVE MovesBaseFrom[COUNT_SQUARES];
	MG_JUMPTARGETSINDEX TargetIndex;
#if defined(CM_ALIGNMENT_CACHELINE) && (CM_ALIGNMENT_CACHELINE >= 64)
	std::uint8_t Padding[7];                  
#endif
};

void JUMPTABLE_Initialize_QuietMoves(const MG_PLAYER& player, const MG_PIECETYPE& piece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable);

void JUMPTABLE_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& movingPiece, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable);

MG_MOVE JUMPTABLE_CountMoves(const MG_MOVEGEN* pMoveGen, const int& jumptarget);

void JUMPTABLE_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

void JUMPTABLE_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

BB_BITBOARD JUMPTABLE_GetPieceAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest);

#endif