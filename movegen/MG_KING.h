#pragma once
#ifndef MG_KING_H
#define MG_KING_H

#include "MG_JUMPTABLE.h"
#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

BB_BITBOARD KING_MovesFromSquare(const BB_SQUARE& squareFrom);

MG_MOVE KING_CountMoves(const MG_MOVEGEN* pMoveGen);

void KING_Initialize_Targets(MG_MOVEGEN* pMoveGen);

void KING_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void KING_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void KING_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

void KING_Initialize_CastleMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void KING_GenerateCastleMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

#endif