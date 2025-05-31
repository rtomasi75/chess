#pragma once
#ifndef SE_CONTROLFLAGSFLAGS_H
#define SE_CONTROLFLAGSFLAGS_H

#include <cstdint>
#include <atomic>

typedef std::uint32_t SE_CONTROLFLAGS;
typedef std::atomic<SE_CONTROLFLAGS> SE_ATOMICCONTROLFLAGS;

#define CONTROLFLAGS_NONE			UINT32_C(0x00000000)
#define CONTROLFLAGS_ACTIVE			UINT32_C(0x40000000)
#define CONTROLFLAGS_ROOT			UINT32_C(0x80000000)
#define CONTROLFLAGS_INITIALIZED	UINT32_C(0x20000000)
#define CONTROLFLAGS_READY      	UINT32_C(0x10000000)
#define CONTROLFLAGS_RUNNING      	UINT32_C(0x08000000)

#define CONTROLFLAGS_IS_READY(flags) (((flags).load(std::memory_order_seq_cst ) & CONTROLFLAGS_READY) != 0)
#define CONTROLFLAGS_IS_ACTIVE(flags) (((flags).load(std::memory_order_seq_cst ) & CONTROLFLAGS_ACTIVE) != 0)
#define CONTROLFLAGS_IS_ROOT(flags) (((flags).load(std::memory_order_seq_cst ) & CONTROLFLAGS_ROOT) != 0)
#define CONTROLFLAGS_IS_INITIALIZED(flags) (((flags).load(std::memory_order_seq_cst ) & CONTROLFLAGS_INITIALIZED) != 0)
#define CONTROLFLAGS_IS_RUNNING(flags) (((flags).load(std::memory_order_seq_cst ) & CONTROLFLAGS_RUNNING) != 0)

#define CONTROLFLAGS_SET_READY(flags) (void)((flags).fetch_or(CONTROLFLAGS_READY, std::memory_order_seq_cst ))
#define CONTROLFLAGS_SET_ACTIVE(flags) (void)((flags).fetch_or(CONTROLFLAGS_ACTIVE, std::memory_order_seq_cst ))
#define CONTROLFLAGS_SET_ROOT(flags) (void)((flags).fetch_or(CONTROLFLAGS_ROOT, std::memory_order_seq_cst ))
#define CONTROLFLAGS_SET_INITIALIZED(flags) (void)((flags).fetch_or(CONTROLFLAGS_INITIALIZED, std::memory_order_seq_cst ))
#define CONTROLFLAGS_SET_RUNNING(flags) (void)((flags).fetch_or(CONTROLFLAGS_RUNNING, std::memory_order_seq_cst ))

#define CONTROLFLAGS_CLEAR(flags) (void)((flags).store(CONTROLFLAGS_NONE, std::memory_order_seq_cst ))

#define CONTROLFLAGS_CLEAR_READY(flags) (void)((flags).fetch_and(~CONTROLFLAGS_READY, std::memory_order_seq_cst ))
#define CONTROLFLAGS_CLEAR_ACTIVE(flags) (void)((flags).fetch_and(~CONTROLFLAGS_ACTIVE, std::memory_order_seq_cst ))
#define CONTROLFLAGS_CLEAR_ROOT(flags) (void)((flags).fetch_and(~CONTROLFLAGS_ROOT, std::memory_order_seq_cst ))
#define CONTROLFLAGS_CLEAR_INITIALIZED(flags) (void)((flags).fetch_and(~CONTROLFLAGS_INITIALIZED, std::memory_order_seq_cst ))
#define CONTROLFLAGS_CLEAR_RUNNING(flags) (void)((flags).fetch_and(~CONTROLFLAGS_RUNNING, std::memory_order_seq_cst ))

#endif // SE_CONTROLFLAGSFLAGS_H

