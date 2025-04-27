#pragma once
#ifndef CM_INTRINSICS_H
#define CM_INTRINSICS_H

#include <cstdint>

std::int8_t CM_BitScanForward(const std::uint64_t& bb);

std::int8_t CM_BitScanReverse(std::uint64_t bb);

std::int8_t CM_PopulationCount(const std::uint64_t& bb);

std::uint64_t CM_BitDeposit(const std::uint64_t& val, std::uint64_t mask);

std::uint64_t CM_BitExtract(const std::uint64_t& val, std::uint64_t mask);



#endif