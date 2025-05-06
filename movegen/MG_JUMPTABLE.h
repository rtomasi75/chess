#pragma once
#ifndef MG_JUMPTABLE_H
#define MG_JUMPTABLE_H

#include "libBitboard.h"
#include "MG_MOVE.h"

typedef std::int16_t MG_JUMPTARGETSINDEX;

struct MG_MOVEGEN;

struct MG_JUMPTABLE
{
	MG_MOVE MovesBaseFrom[COUNT_SQUARES];
	MG_JUMPTARGETSINDEX TargetIndex;
};

void JUMPTABLE_Initialize_QuietMoves(const MG_PLAYER& player, const MG_PIECETYPE& piece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable);

void JUMPTABLE_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& movingPiece, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable);

MG_MOVE JUMPTABLE_CountMoves(const MG_MOVEGEN* pMoveGen, const int& jumptarget);


#endif