#pragma once
#ifndef MG_QUEEN_H
#define MG_QUEEN_H

#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "MG_SLIDELOOKUP.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

MG_MOVE QUEEN_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer);

void QUEEN_Initialize_LookUps(MG_MOVEGEN* pMoveGen);

void QUEEN_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void QUEEN_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void QUEEN_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

#endif