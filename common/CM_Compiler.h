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

#if defined(__GNUC__) || defined(__clang__)
#define CM_PREFETCH(addr) __builtin_prefetch(addr, 0, 3)
#elif defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#include <xmmintrin.h>
#define CM_PREFETCH(addr) _mm_prefetch((const char*)(addr), _MM_HINT_T0)
#else
#define CM_PREFETCH(addr) // No prefetch on unsupported platforms
#endif

#endif

// Branchless, ISO-compliant conditional prefetch macro
#define CM_PREFETCH_COND(cond, ptr)                          \
    do {                                                    \
        const uintptr_t _prefetchAddrInt =                  \
            (uintptr_t)(ptr) * (uintptr_t)(!!(cond));       \
        CM_PREFETCH((const void*)_prefetchAddrInt);         \
    } while(0)