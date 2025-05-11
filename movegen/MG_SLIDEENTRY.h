#pragma once
#ifndef MG_SLIDEENTRY_H
#define MG_SLIDEENTRY_H

#include "libBitboard.h"

#define MOVEGEN_COMPACT_TARGETS

typedef std::int32_t MG_SLIDEENTRYINDEX;

#ifdef MOVEGEN_COMPACT_TARGETS

typedef std::uint16_t MG_COMPRESSEDTARGETS;

#endif

struct MG_SLIDEENTRY
{
#ifdef MOVEGEN_COMPACT_TARGETS
	MG_COMPRESSEDTARGETS CompressedTargets;
#else
	BB_BITBOARD Targets;
#endif
};

#endif