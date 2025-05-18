#pragma once
#ifndef MG_PIECEINFO_H
#define MG_PIECEINFO_H

#include "MG_MOVEMECHANIC.h"
#include "MG_MOVETYPE.h"
#include "MG_PIECETYPE.h"
#include "../libCommon.h"

#define COUNT_TABLEINDICES (COUNT_PIECETYPES+2)
#define TABLEINDEX_QUIET 0
#define TABLEINDEX_SPECIAL 1
#define TABLEINDEX_CAPTURE(_PIECE_) (2+(_PIECE_))

typedef std::int8_t MG_TABLEINDEX;

struct CM_ALIGN_CACHELINE MG_PIECEINFO
{
	MG_TABLEINDEX TableIndex[COUNT_TABLEINDICES];
	CM_BOOL IsRoyal;
	std::uint8_t Padding[3];
	MG_MOVEMECHANIC MoveMechanic[COUNT_MOVETYPES];
};

#endif