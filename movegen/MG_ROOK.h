#pragma once
#ifndef MG_ROOK_H
#define MG_ROOK_H

#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "libBitboard.h"

struct MG_MOVEGEN;

BB_BITBOARD ROOK_QuietMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD ROOK_CaptureMovesFromSquare(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

size_t ROOK_CountMoves(const MG_MOVEGEN* pMoveGen);

void ROOK_Initialize_LookUps(MG_MOVEGEN* pMoveGen);

void ROOK_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void ROOK_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void ROOK_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

#define COUNT_KINGMOVES (COUNT_SQUARES*8*(1+COUNT_PIECETYPES))

#endif