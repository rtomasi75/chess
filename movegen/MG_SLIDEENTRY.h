#pragma once
#ifndef MG_SLIDEENTRY_H
#define MG_SLIDEENTRY_H

#include "libBitboard.h"

typedef std::int32_t MG_SLIDEENTRYINDEX;

struct MG_SLIDEENTRY
{
	BB_BITBOARD Targets;
	BB_BITBOARD Occupancy;
};

#endif