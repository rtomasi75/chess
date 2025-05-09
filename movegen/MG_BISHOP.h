#pragma once
#ifndef MG_BISHOP_H
#define MG_BISHOP_H

#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "MG_SLIDELOOKUP.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

BB_BITBOARD BISHOP_QuietMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD BISHOP_CaptureMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

MG_MOVE BISHOP_CountMoves(const MG_MOVEGEN* pMoveGen);

void BISHOP_Initialize_LookUps(MG_MOVEGEN* pMoveGen);

void BISHOP_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void BISHOP_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void BISHOP_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

#endif