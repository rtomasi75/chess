#pragma once
#ifndef MG_SLIDELOOKUP_H
#define MG_SLIDELOOKUP_H

#include <cstdint>

#include "libBitboard.h"

#include "MG_SLIDEMASKS.h"
#include "MG_MOVE.h"

struct MG_MOVEGEN;

typedef std::int32_t MG_SLIDEENTRYINDEX;
struct MG_SLIDELOOKUP
{
	MG_SLIDEMASKINDEX CountMasks;
	MG_SLIDEMASKINDEX MaskIndex[COUNT_SLIDEMASKS];
	MG_SLIDEENTRYINDEX EntryBase[COUNT_SLIDEMASKS];
	MG_MOVE MoveBase;
	MG_MOVE CaptureMoveBase[COUNT_PIECETYPES];
};

size_t SLIDELOOKUP_CountSlideEntries(const MG_MOVEGEN* pMoveGen);

#endif