#pragma once
#ifndef MG_SLIDEENTRY_H
#define MG_SLIDEENTRY_H

#include "libBitboard.h"

#define MOVEGEN_COMPACT_TARGETS

typedef std::int32_t MG_SLIDEENTRYINDEX;

#ifdef MOVEGEN_COMPACT_TARGETS

typedef std::uint16_t MG_COMPRESSEDTARGETS;

#endif

struct CM_ALIGN_PROCWORD MG_SLIDEENTRY
{
#ifdef MOVEGEN_COMPACT_TARGETS
    MG_COMPRESSEDTARGETS CompressedTargets;       // 2 bytes
#if defined(CM_ALIGNMENT_PROCWORD) && (CM_ALIGNMENT_PROCWORD >= 8)
    std::uint8_t Padding[6];                      // Padding to ensure 8-byte alignment
#endif
#else
    BB_BITBOARD Targets;                          // 8 bytes (already aligned)
#endif
};

#endif