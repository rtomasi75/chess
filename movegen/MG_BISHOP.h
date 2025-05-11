#pragma once
#ifndef MG_BISHOP_H
#define MG_BISHOP_H

#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "MG_SLIDELOOKUP.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

MG_MOVE BISHOP_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer);

void BISHOP_Initialize_LookUps(MG_MOVEGEN* pMoveGen);

void BISHOP_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void BISHOP_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void BISHOP_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

#endif