#pragma once
#ifndef MG_KNIGHT_H
#define MG_KNIGHT_H


#include "MG_JUMPTABLE.h"
#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

BB_BITBOARD KNIGHT_MovesFromSquare(const BB_SQUARE& squareFrom);

MG_MOVE KNIGHT_CountMoves(const MG_MOVEGEN* pMoveGen);

void KNIGHT_Initialize_Targets(MG_MOVEGEN* pMoveGen);

void KNIGHT_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void KNIGHT_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void KNIGHT_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);


#endif