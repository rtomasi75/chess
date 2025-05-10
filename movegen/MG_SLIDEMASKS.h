#pragma once
#ifndef MG_SLIDEMASKS_H
#define MG_SLIDEMASKS_H

#include "libBitboard.h"
#include "MG_SLIDEENTRY.h"
#include "MG_PLAYER.h"
#include "MG_PIECETYPE.h"
#include "MG_MOVE.h"

typedef std::int8_t MG_SLIDEMASKINDEX;

typedef std::int16_t MG_SLIDETARGETINDEX;

struct MG_SLIDEMASKS
{
	BB_BITBOARD Mask[COUNT_SQUARES];
	BB_BITBOARD PotentialTargets[COUNT_SQUARES];
	MG_SLIDEENTRYINDEX BaseEntry[COUNT_SQUARES];
	MG_SLIDEENTRYINDEX CountEntries[COUNT_SQUARES];
	BB_SQUARECOUNT CountMaskBits[COUNT_SQUARES];
	BB_SQUARECOUNT CountPotentialTargetsBits[COUNT_SQUARES];
	MG_SLIDETARGETINDEX CountPotentialTargets[COUNT_SQUARES];
};

struct MG_MOVEGEN;

#define COUNT_SLIDEMASKS 2
#define SLIDEMASKS_HORIZONTAL 0
#define SLIDEMASKS_DIAGONAL 1


BB_BITBOARD SLIDEMASKS_QuietMovesFromSquareHorizontal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_QuietMovesFromSquareDiagonal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_CaptureMovesFromSquareHorizontal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_CaptureMovesFromSquareDiagonal(const BB_SQUARE& squareFrom, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_GenerateTargetsHorizontal(const BB_SQUARE& square, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_GenerateTargetsDiagonal(const BB_SQUARE& square, const BB_BITBOARD& occupancy);

BB_BITBOARD SLIDEMASKS_GenerateMaskHorizontal(const BB_SQUARE& square);

BB_BITBOARD SLIDEMASKS_GenerateMaskDiagonal(const BB_SQUARE& square);

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntriesDiagonal();

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntriesHorizontal();

MG_SLIDEENTRYINDEX SLIDEMASKS_CountEntries();

void SLIDEMASKS_Initialize(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry);

void SLIDEMASKS_InitializeDiagonal(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry);

void SLIDEMASKS_InitializeHorizontal(MG_MOVEGEN* pMoveGen, MG_SLIDEENTRYINDEX& nextEntry);

void SLIDEMASKS_Initialize_QuietMoves(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

void SLIDEMASKS_Initialize_CaptureMoves(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove);

#endif