#pragma once
#ifndef SE_TYPES_H
#define SE_TYPES_H

#include <cstdint>


typedef std::uint64_t SE_POSITIONCOUNT;
typedef std::uint64_t SE_LEAFCOUNT;
typedef std::int8_t SE_DEPTH;
typedef std::int16_t SE_THREADINDEX;

#define THREADINDEX_NONE INT16_C(-1)

#include "SE_CONTROLFLAGS.h"
#include "SE_NODEFLAGS.h"
#include "SE_NODESTATE.h"

#define SEARCH_MAX_DEPTH 64

#endif // SE_TYPES_H