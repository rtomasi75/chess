#include "CM_Intrinsics.h"

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <atomic>
#include <bit>

// Internal flags (initialized at runtime)
static std::atomic<bool> g_hasPOPCNT{ false };
static std::atomic<bool> g_hasBMI2{ false };
static std::atomic<bool> g_hasBMI1{ false };

#define CM_DEBRUIJN64 UINT64_C(0x03f79d71b4cb0a89)

static const std::int8_t CM_Index64[64] =
{
	INT8_C(0),  INT8_C(1),  INT8_C(48), INT8_C(2),  INT8_C(57), INT8_C(49), INT8_C(28), INT8_C(3),
	INT8_C(61), INT8_C(58), INT8_C(50), INT8_C(42), INT8_C(38), INT8_C(29), INT8_C(17), INT8_C(4),
	INT8_C(62), INT8_C(55), INT8_C(59), INT8_C(36), INT8_C(53), INT8_C(51), INT8_C(43), INT8_C(22),
	INT8_C(45), INT8_C(39), INT8_C(33), INT8_C(30), INT8_C(24), INT8_C(18), INT8_C(12), INT8_C(5),
	INT8_C(63), INT8_C(47), INT8_C(56), INT8_C(27), INT8_C(60), INT8_C(41), INT8_C(37), INT8_C(16),
	INT8_C(54), INT8_C(35), INT8_C(52), INT8_C(21), INT8_C(44), INT8_C(32), INT8_C(23), INT8_C(11),
	INT8_C(46), INT8_C(26), INT8_C(40), INT8_C(15), INT8_C(34), INT8_C(20), INT8_C(31), INT8_C(10),
	INT8_C(25), INT8_C(14), INT8_C(19), INT8_C(9),  INT8_C(13), INT8_C(8),  INT8_C(7),  INT8_C(6)
};

void CM_DetectIntrinsics()
{
	int cpuInfo[4]{};
	__cpuid(cpuInfo, 1);
	g_hasPOPCNT = (cpuInfo[2] & (1 << 23)) != 0;

	__cpuid(cpuInfo, 7);
	g_hasBMI2 = (cpuInfo[1] & (1 << 8)) != 0;

	__cpuid(cpuInfo, 7);
	g_hasBMI1 = (cpuInfo[1] & (1 << 3)) != 0;
}

bool CM_HasPOPCNT()
{
#if CM_HAVE_POPCNT
	return g_hasPOPCNT.load();
#else
	return false;
#endif
}

bool CM_HasBMI2()
{
#if CM_HAVE_BMI2
	return g_hasBMI2.load();
#else
	return false;
#endif
}

bool CM_HasBMI1()
{
#if CM_HAVE_BMI1
	return g_hasBMI1.load();
#else
	return false;
#endif
}

// Fallbacks (portable implementations)
std::int8_t CM_PopulationCountFallback(std::uint64_t value)
{
	value = value - ((value >> 1) & UINT64_C(0x5555555555555555));
	value = (value & UINT64_C(0x3333333333333333)) + ((value >> 2) & UINT64_C(0x3333333333333333));
	value = (value + (value >> 4)) & UINT64_C(0x0F0F0F0F0F0F0F0F);
	return (std::int8_t)((value * UINT64_C(0x0101010101010101)) >> 56);
}

std::uint64_t CM_BitDepositFallback(std::uint64_t value, std::uint64_t mask)
{
	std::uint64_t result = 0;
	for (std::uint64_t bb = 1; mask != 0; bb <<= 1)
	{
		if (value & bb)
			result |= mask & (std::uint64_t)-(std::int64_t)mask;
		mask &= (mask - 1);
	}
	return result;
}

std::uint64_t CM_BitExtractFallback(std::uint64_t value, std::uint64_t mask)
{
	std::uint64_t result = 0;
	std::uint64_t bb = 1;
	for (std::uint64_t m = mask; m != 0; m &= (m - 1))
	{
		std::uint64_t bit = m & (std::uint64_t)-(std::int64_t)m;
		if (value & bit)
			result |= bb;
		bb <<= 1;
	}
	return result;
}

std::int8_t CM_PopulationCount(std::uint64_t value)
{
#if CM_HAVE_POPCNT
	if (g_hasPOPCNT)
		return (std::int8_t)_mm_popcnt_u64(value);
#endif
	return CM_PopulationCountFallback(value);
}

std::uint64_t CM_BitDeposit(std::uint64_t value, std::uint64_t mask)
{
#if CM_HAVE_BMI2
	if (g_hasBMI2)
		return _pdep_u64(value, mask);
#endif
	return CM_BitDepositFallback(value, mask);
}

std::uint64_t CM_BitExtract(std::uint64_t value, std::uint64_t mask)
{
#if CM_HAVE_BMI2
	if (g_hasBMI2)
		return _pext_u64(value, mask);
#endif
	return CM_BitExtractFallback(value, mask);
}

std::string CM_GetIntrinsicInfo()
{
	std::string result = "Runtime CPU features:";
	result += CM_HasPOPCNT() ? " POPCNT" : "";
	result += CM_HasBMI1() ? " BMI1" : "";
	result += CM_HasBMI2() ? " BMI2" : "";
	if (result == "Runtime CPU features:")
		result += " (none)";
	return result;
}

std::int8_t CM_BitScanForwardFallback(std::uint64_t value)
{
	return CM_Index64[((value & (std::uint64_t)-(std::int64_t)value) * CM_DEBRUIJN64) >> 58];
}

std::int8_t CM_BitScanForward(std::uint64_t value)
{
#if CM_HAVE_BMI1
#if defined(_MSC_VER)
	if (g_hasBMI1)
	{
		unsigned long index;
		_BitScanForward64(&index, value);
		return static_cast<std::int8_t>(index);
	}
#else
	if (g_hasBMI1)
		return static_cast<std::int8_t>(_tzcnt_u64(value));
#endif
#endif
	return CM_BitScanForwardFallback(value);
}