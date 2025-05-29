#pragma once
#ifndef CM_COMPILER_H
#define CM_COMPILER_H

#ifndef __MSCVER__

#define abstract =0

#endif

#ifndef CM_ALIGNMENT_CACHELINE
#define CM_ALIGNMENT_CACHELINE 64
#endif

#ifndef CM_ALIGNMENT_PROCWORD
#define CM_ALIGNMENT_PROCWORD 8
#endif

#define CM_ALIGN_CACHELINE alignas(CM_ALIGNMENT_CACHELINE)
#define CM_ALIGN_PROCWORD alignas(CM_ALIGNMENT_PROCWORD)

// Prefetch macros (adjustable for hint level)
#if defined(__GNUC__) || defined(__clang__)
#define CM_PREFETCH_L1(addr) __builtin_prefetch((addr), 0, 3)
#define CM_PREFETCH_L2(addr) __builtin_prefetch((addr), 0, 2)
#define CM_PREFETCH_L3(addr) __builtin_prefetch((addr), 0, 1)
#elif defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#include <xmmintrin.h>
#define CM_PREFETCH_L1(addr) _mm_prefetch((const char*)(addr), _MM_HINT_T0)
#define CM_PREFETCH_L2(addr) _mm_prefetch((const char*)(addr), _MM_HINT_T1)
#define CM_PREFETCH_L3(addr) _mm_prefetch((const char*)(addr), _MM_HINT_T2)
#else
#define CM_PREFETCH_L1(addr)
#define CM_PREFETCH_L2(addr)
#define CM_PREFETCH_L3(addr)
#endif

// Backwards compatibility — prefer CM_PREFETCH_Lx instead for control, this will stay supported
#define CM_PREFETCH(addr) CM_PREFETCH_L1(addr)

// Branchless, hint-level specific conditional prefetch
#define CM_PREFETCH_COND_L1(cond, ptr) do { const uintptr_t _prefetchAddrInt = (uintptr_t)(ptr) * (uintptr_t)(!!(cond)); CM_PREFETCH_L1((const void*)_prefetchAddrInt); } while (0)

#define CM_PREFETCH_COND_L2(cond, ptr) do { const uintptr_t _prefetchAddrInt = (uintptr_t)(ptr) * (uintptr_t)(!!(cond)); CM_PREFETCH_L2((const void*)_prefetchAddrInt); } while (0)

#define CM_PREFETCH_COND_L3(cond, ptr) do { const uintptr_t _prefetchAddrInt = (uintptr_t)(ptr) * (uintptr_t)(!!(cond)); CM_PREFETCH_L3((const void*)_prefetchAddrInt); } while (0)

// Backwards compatibility — prefer CM_PREFETCH_COND_Lx instead for control, this will stay supported
#define CM_PREFETCH_COND(cond, ptr) do { const uintptr_t _prefetchAddrInt = (uintptr_t)(ptr) * (uintptr_t)(!!(cond)); CM_PREFETCH((const void*)_prefetchAddrInt); } while (0)

#endif

#ifdef _DEBUG
//#define CM_TRACE(fmt, ...) do { std::fprintf(stderr, fmt "\n", ##__VA_ARGS__); std::fflush(stderr); } while (0)
#define CM_TRACE(fmt, ...) do { std::printf(fmt "\n", ##__VA_ARGS__); std::fflush(stderr); } while (0)
#else
#define CM_TRACE(fmt, ...) do { } while (0)
#endif