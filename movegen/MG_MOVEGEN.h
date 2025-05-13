#pragma once
#ifndef MG_SETUP_H
#define MG_SETUP_H

#include "MG_JUMPTABLE.h"
#include "MG_SLIDEMASKS.h"
#include "MG_SLIDELOOKUP.h"
#include "MG_PLAYER.h"
#include "MG_PIECETYPE.h"
#include "MG_PIECEINFO.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVE.h"
#include "MG_KING.h"
#include "MG_POSITION.h"
#include "MG_MOVELIST.h"
#include "MG_MOVEDATA.h"
#include "MG_SLIDEENTRY.h"
#include "MG_PAWN.h"

#define JUMPTABLE_KING_QUIET 0
#define JUMPTABLE_KING_CAPTURE(_PIECE_) (JUMPTABLE_KING_QUIET+(_PIECE_)+1)
#define JUMPTABLE_KNIGHT_QUIET JUMPTABLE_KING_CAPTURE(COUNT_PIECETYPES)
#define JUMPTABLE_KNIGHT_CAPTURE(_PIECE_) (JUMPTABLE_KNIGHT_QUIET+(_PIECE_)+1)

#define COUNT_JUMPTABLES (2*(1+COUNT_PIECETYPES))

#define JUMPTARGETS_KING 0
#define JUMPTARGETS_KNIGHT 1

#define COUNT_JUMPTARGETS 2

#define SLIDELOOKUP_ROOK_QUIET 0
#define SLIDELOOKUP_ROOK_CAPTURE 1
#define SLIDELOOKUP_BISHOP_QUIET 2
#define SLIDELOOKUP_BISHOP_CAPTURE 3
#define SLIDELOOKUP_QUEEN_QUIET 4
#define SLIDELOOKUP_QUEEN_CAPTURE 5

#define COUNT_SLIDELOOKUPS 6

#define MOVGEN_LEGALMOVES

typedef std::int8_t MG_OPTIONINDEX;

struct MG_MOVEGEN
{
	MG_PIECEINFO PieceInfo[COUNT_PLAYERS][COUNT_PIECETYPES];
	MG_SLIDEMASKS SlideMasks[COUNT_SLIDEMASKS];
	MG_JUMPTABLE JumpTable[COUNT_JUMPTABLES];
	MG_SLIDELOOKUP SlideLookUp[COUNT_SLIDELOOKUPS];
	BB_BITBOARD JumpTargets[COUNT_JUMPTARGETS][COUNT_SQUARES];
	MG_PAWNTABLE PawnTable[COUNT_PLAYERS];
	MG_MOVEINFO* MoveTable[COUNT_PLAYERS];
	MG_SLIDEENTRY* SlideEntries;
	MG_MOVE CountMoves[COUNT_PLAYERS];
	MG_MOVE CastleBase[COUNT_PLAYERS];
	MG_SLIDEENTRYINDEX CountSlideEntries;
};

MG_MOVE MOVEGEN_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer);

void MOVEGEN_Initialize(MG_MOVEGEN* pMoveGen);

void MOVEGEN_Deinitialize(MG_MOVEGEN* pMoveGen);

MG_OPTIONINDEX MOVEGEN_OptionIndex(const BB_SQUARE& square, const BB_BITBOARD& targets);

void MOVEGEN_GenerateMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, MG_MOVELIST* pMoveList);

bool MOVEGEN_ParseMoveString(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVELIST* pMoveList, const char* pString, const int& len, int& strPos, MG_MOVE& outParsed);

void MOVEGEN_MakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_MOVEDATA* pOutMoveData, MG_POSITION* pPosition);

void MOVEGEN_UnmakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_MOVEDATA* pMoveData, MG_POSITION* pPosition);

void MOVEGEN_MakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_POSITION* pPosition);

void MOVEGEN_UnmakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_POSITION* pPosition);

BB_BITBOARD MOVEGEN_GetPieceAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest);

void MOVEGEN_RecomputeAttacks(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PLAYER& player);

void MOVEGEN_FinalizeMove(const MG_MOVEGEN* pMoveGen, MG_MOVELIST* pMoveList, MG_POSITION* pPosition, const MG_MOVE& move);

#endif