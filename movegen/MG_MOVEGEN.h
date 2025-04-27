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

#define JUMPTABLE_KING_QUIET 0
#define JUMPTABLE_KING_CAPTURE(_PIECE_) (JUMPTABLE_KING_QUIET+(_PIECE_)+1)

#define COUNT_JUMPTABLES (1+COUNT_PIECETYPES)

#define JUMPTARGETS_KING 0

#define COUNT_JUMPTARGETS 1

#define SLIDELOOKUP_ROOK_QUIET 0
#define SLIDELOOKUP_ROOK_CAPTURE 1

#define COUNT_SLIDELOOKUPS 2

typedef std::int8_t MG_OPTIONINDEX;

#define INDEX_KINGMOVES 1

struct MG_MOVEGEN
{
	MG_PIECEINFO PieceInfo[COUNT_PLAYERS][COUNT_PIECETYPES];
	MG_SLIDEMASKS SlideMasks[COUNT_SLIDEMASKS];
	MG_JUMPTABLE JumpTable[COUNT_JUMPTABLES];
	MG_SLIDELOOKUP SlideLookUp[COUNT_SLIDELOOKUPS];
	BB_BITBOARD JumpTargets[COUNT_JUMPTARGETS][COUNT_SQUARES];
	MG_MOVEINFO* MoveTable[COUNT_PLAYERS];
	MG_SLIDEENTRY* SlideEntries[COUNT_PLAYERS];
	size_t CountMoves;
	size_t CountSlideEntries;
};

size_t MOVEGEN_CountMoves(const MG_MOVEGEN* pMoveGen);

void MOVEGEN_Initialize(MG_MOVEGEN* pMoveGen);

void MOVEGEN_Deinitialize(MG_MOVEGEN* pMoveGen);

MG_OPTIONINDEX MOVEGEN_OptionIndex(const BB_SQUARE& square, const BB_BITBOARD& targets);

void MOVEGEN_GenerateMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, MG_MOVELIST* pMoveList);

void MOVEGEN_GenerateQuietJumps(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

void MOVEGEN_GenerateQuietSlides(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

void MOVEGEN_GenerateCaptureJumps(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

void MOVEGEN_GenerateCaptureSlides(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList);

bool MOVEGEN_ParseMoveString(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVELIST* pMoveList, const char* pString, const int& len, int& strPos, MG_MOVE& outParsed);

void MOVEGEN_MakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_MOVEDATA* pOutMoveData, MG_POSITION* pPosition);

void MOVEGEN_UnmakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_MOVEDATA* pMoveData, MG_POSITION* pPosition);

#endif