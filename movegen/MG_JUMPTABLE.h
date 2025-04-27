#pragma once
#ifndef MG_JUMPTABLE_H
#define MG_JUMPTABLE_H

#include "libBitboard.h"
#include "MG_MOVE.h"

typedef std::int16_t MG_JUMPTARGETSINDEX;

struct MG_JUMPTABLE
{
	MG_MOVE MovesBaseFrom[COUNT_SQUARES];
	MG_JUMPTARGETSINDEX TargetIndex;
};

#endif