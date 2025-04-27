#pragma once
#ifndef SE_GAME_H
#define SE_GAME_H

#include "libMovegen.h"
#include "SE_PLAYEDMOVE.h"
#include <cstdint>


typedef std::int16_t SE_PLAYEDMOVEINDEX;
#define GAME_MAX_PLAYEDMOVES INT16_C(4096)

struct SE_GAME
{
	MG_POSITION CurrentPosition;
	MG_MOVELIST LegalMoves;
	SE_PLAYEDMOVEINDEX CurrentMove;
	SE_PLAYEDMOVE PlayedMoves[GAME_MAX_PLAYEDMOVES];
};

void GAME_Initialize(SE_GAME* pGame, const MG_MOVEGEN* pMoveGen);

void GAME_MakeMove(SE_GAME* pGame, const MG_MOVE& move, const MG_MOVEGEN* pMoveGen);

void GAME_UnmakeMove(SE_GAME* pGame, const MG_MOVEGEN* pMoveGen);


#endif