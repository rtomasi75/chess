#pragma once
#ifndef MG_PAWN_H
#define MG_PAWN_H

#include "MG_PIECEINFO.h"
#include "MG_MOVE.h"
#include "libBitboard.h"

struct MG_MOVEGEN;
struct MG_POSITION;
struct MG_MOVELIST;


struct MG_PAWNTABLE
{
	MG_MOVE QuietBase;
	MG_MOVE CaptureBase;
	MG_MOVE PromotionBase;
	MG_MOVE PromoCaptureBase;
};

MG_MOVE PAWN_CountQuietMoves();

MG_MOVE PAWN_CountCaptureMoves();

MG_MOVE PAWN_CountMoves();

void PAWN_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo);

void PAWN_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void PAWN_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void PAWN_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

void PAWN_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

#endif