#pragma once

#include <cstdint>
#include <string>

// Compile-time feature macros (defined by compiler flags or intrinsics headers)
#if defined(__GNUC__) || defined(__clang__)
#define CM_HAVE_MSC 0
#include <x86intrin.h>
#if defined(__POPCNT__)
#define CM_HAVE_POPCNT 1
#else
#define CM_HAVE_POPCNT 0
#endif
#if defined(__BMI2__)
#define CM_HAVE_BMI2 1
#else
#define CM_HAVE_BMI2 0
#endif
#if defined(__BMI__)
#define CM_HAVE_BMI 1
#else
#define CM_HAVE_BMI 0
#endif
#elif defined(_MSC_VER)
#define CM_HAVE_MSC 1
#include <intrin.h>
#define CM_HAVE_POPCNT 1
#define CM_HAVE_BMI2   1
#define CM_HAVE_BMI   1
#else
#define CM_HAVE_MSC 0
#define CM_HAVE_POPCNT 0
#define CM_HAVE_BMI2   0
#define CM_HAVE_BMI   0
#endif

// Detect CPU support at runtime — call during program startup
void CM_DetectIntrinsics();

// Runtime feature queries
bool CM_HasPOPCNT();
bool CM_HasBMI2();
bool CM_HasBMI();

// Public API — routes to intrinsic or fallback depending on runtime detection
std::int8_t CM_PopulationCount(std::uint64_t value);
std::uint64_t CM_BitDeposit(std::uint64_t value, std::uint64_t mask);
std::uint64_t CM_BitExtract(std::uint64_t value, std::uint64_t mask);
std::int8_t CM_BitScanForward(std::uint64_t value);
bool CM_PopLsb(std::uint64_t& value, std::int8_t& bit);

// Debug string for displaying detected feature support
std::string CM_GetIntrinsicInfo();
