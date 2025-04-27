#pragma once
#ifndef MG_PIECEINFO_H
#define MG_PIECEINFO_H

#include "MG_MOVEMECHANIC.h"
#include "MG_MOVETYPE.h"
#include "MG_PIECETYPE.h"

#define COUNT_TABLEINDICES (COUNT_PIECETYPES+2)
#define TABLEINDEX_QUIET 0
#define TABLEINDEX_SPECIAL 1
#define TABLEINDEX_CAPTURE(_PIECE_) (2+(_PIECE_))

struct MG_PIECEINFO
{
	bool IsRoyal;
	MG_MOVEMECHANIC MoveMechanic[COUNT_MOVETYPES];
	std::uint8_t TableIndex[COUNT_TABLEINDICES];
};

#endif