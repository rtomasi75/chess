#pragma once
#ifndef MG_SLIDELOOKUP_H
#define MG_SLIDELOOKUP_H

#include <cstdint>

#include "libBitboard.h"

#include "MG_SLIDEMASKS.h"
#include "MG_MOVE.h"

struct MG_MOVEGEN;

struct MG_SLIDELOOKUP
{
	MG_SLIDEMASKINDEX CountMasks;
	MG_SLIDEMASKINDEX MaskIndex[COUNT_SLIDEMASKS];
	MG_MOVE MoveBase[COUNT_PLAYERS][COUNT_SLIDEMASKS][COUNT_SQUARES];
};

size_t SLIDELOOKUP_CountSlideEntries(const MG_MOVEGEN* pMoveGen);

#endif