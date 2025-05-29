#pragma once
#ifndef SE_TYPES_H
#define SE_TYPES_H

#include <cstdint>

struct SE_THREAD;
struct MG_MOVEGEN;

typedef std::uint64_t SE_POSITIONCOUNT;
typedef std::uint64_t SE_LEAFCOUNT;
typedef std::int8_t SE_DEPTH;
typedef std::int16_t SE_THREADINDEX;
typedef void* SE_CONTEXTPTR;
typedef void* SE_EXECUTIONTOKEN;
typedef std::int8_t SE_FORKINDEX;

#define FORKINDEX_NONE INT8_C(-1)

#define THREADINDEX_NONE INT16_C(-1)

#include "SE_CONTROLFLAGS.h"
#include "SE_NODEFLAGS.h"
#include "SE_NODESTATE.h"

#define SEARCH_MAX_DEPTH 64

//#define SEARCH_TRACE_DISPATCHER
//#define SEARCH_TRACE_THREAD
//#define SEARCH_TRACE_FSM
#endif // SE_TYPES_H