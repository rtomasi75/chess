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
#if defined(CM_ALIGNMENT_PROCWORD) && (CM_ALIGNMENT_PROCWORD >= 8)
	std::uint8_t Padding[7];
#endif
	CM_ALIGN_PROCWORD MG_MOVEMECHANIC MoveMechanic[COUNT_MOVETYPES];
#if defined(CM_ALIGNMENT_CACHELINE) && (CM_ALIGNMENT_CACHELINE >= 64)
	std::uint8_t PaddingOutside[24]; 
#endif
};

#endif