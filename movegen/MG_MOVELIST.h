#pragma once
#ifndef MG_MOVELIST_H
#define MG_MOVELIST_H

#include "MG_MOVE.h"

typedef std::int16_t MG_MOVEINDEX;

#define MAX_MOVES INT16_C(255)

struct MG_MOVELIST
{
	MG_MOVEINDEX CountMoves;
	MG_MOVE Move[MAX_MOVES];
};

void MOVELIST_Initialize(MG_MOVELIST* pMoveList);

#endif