#pragma once
#ifndef SE_TYPES_H
#define SE_TYPES_H

#include <cstdint>
#include <atomic>

struct SE_THREAD;
struct MG_MOVEGEN;

typedef std::uint64_t SE_POSITIONCOUNT;
typedef std::uint64_t SE_LEAFCOUNT;
typedef std::int8_t SE_DEPTH;
typedef std::int16_t SE_THREADINDEX;
typedef void* SE_CONTEXTPTR;
typedef void* SE_EXECUTIONTOKEN;
typedef std::int8_t SE_FORKINDEX;
typedef std::int8_t SE_RETENTIONSTATE;
typedef std::atomic<std::int8_t> SE_ATOMICRETENTIONSTATE;
typedef std::uint32_t SE_TICKCOUNT;

#define RETENTIONSTATE_AWAKE  INT8_C(0)
#define RETENTIONSTATE_DROWSY   INT8_C(1)
#define RETENTIONSTATE_SLEEPING INT8_C(2)
#define RETENTIONSTATE_HIBERNATING INT8_C(3)

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