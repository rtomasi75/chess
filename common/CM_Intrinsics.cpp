#include "CM_Intrinsics.h"

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <atomic>
#include <bit>

// Internal flags (initialized at runtime)
#if CM_HAVE_MSC
static std::atomic<bool> g_hasPOPCNT{ false };
static std::atomic<bool> g_hasBMI2{ false };
static std::atomic<bool> g_hasBMI{ false };
#endif

#define CM_DEBRUIJN64 UINT64_C(0x03f79d71b4cb0a89)

#if CM_HAVE_MSC || !CM_HAVE_BMI

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

#endif

typedef std::int8_t(*CM_PopCountFn)(std::uint64_t);
typedef std::uint64_t(*CM_BitDepFn)(std::uint64_t, std::uint64_t);
typedef std::uint64_t(*CM_BitExtFn)(std::uint64_t, std::uint64_t);
typedef std::int8_t(*CM_BsfFn)(std::uint64_t);
typedef bool(*CM_PopLsbFn)(std::uint64_t&, std::int8_t&);

#if CM_HAVE_MSC || !CM_HAVE_POPCNT 

// Fallbacks (portable implementations)
static std::int8_t CM_PopulationCountFallback(std::uint64_t value)
{
	value = value - ((value >> 1) & UINT64_C(0x5555555555555555));
	value = (value & UINT64_C(0x3333333333333333)) + ((value >> 2) & UINT64_C(0x3333333333333333));
	value = (value + (value >> 4)) & UINT64_C(0x0F0F0F0F0F0F0F0F);
	return (std::int8_t)((value * UINT64_C(0x0101010101010101)) >> 56);
}

#endif

#if CM_HAVE_POPCNT
static std::int8_t CM_PopulationCountIntrinsic(std::uint64_t value)
{
	return (std::int8_t)_mm_popcnt_u64(value);
}
#endif

#if CM_HAVE_MSC || !CM_HAVE_BMI2

static std::uint64_t CM_BitDepositFallback(std::uint64_t value, std::uint64_t mask)
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

static std::uint64_t CM_BitExtractFallback(std::uint64_t value, std::uint64_t mask)
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

#endif

#if CM_HAVE_BMI2
static std::uint64_t CM_BitDepositIntrinsic(std::uint64_t value, std::uint64_t mask)
{
	return _pdep_u64(value, mask);
}

static std::uint64_t CM_BitExtractIntrinsic(std::uint64_t value, std::uint64_t mask)
{
	return _pext_u64(value, mask);
}
#endif

#if CM_HAVE_MSC || !CM_HAVE_BMI

static std::int8_t CM_BitScanForwardFallback(std::uint64_t value)
{
	return CM_Index64[((value & (std::uint64_t)-(std::int64_t)value) * CM_DEBRUIJN64) >> 58];
}

static bool CM_PopLsbFallback(std::uint64_t& value, std::int8_t& bit)
{
	if (value)
	{
		bit = CM_Index64[((value & (std::uint64_t)-(std::int64_t)value) * CM_DEBRUIJN64) >> 58];
		value &= (std::uint64_t)(((std::int64_t)value) - 1);
		return true;
	}
	else
		return false;
}

#endif

#if CM_HAVE_BMI
static std::int8_t CM_BitScanForwardIntrinsic(std::uint64_t value)
{
#if defined(_MSC_VER)
	unsigned long index;
	_BitScanForward64(&index, value);
	return static_cast<std::int8_t>(index);
#else
	return static_cast<std::int8_t>(_tzcnt_u64(value));
#endif
}

static bool CM_PopLsbIntrinsic(std::uint64_t& value, std::int8_t& bit)
{
#if CM_HAVE_MSC
	unsigned long index;
	unsigned char ret = _BitScanForward64(&index, value);
	bit = (std::int8_t)(index);
	value ^= _blsi_u64(value);
	return (bool)ret;
#else
	if (value)
	{
		std::uint64_t lsb = _blsi_u64(value);   // isolate LSB
		bit = (std::int8_t)_tzcnt_u64(value);     // tzcnt counts trailing zeros
		value ^= lsb;
		return true;
	}
	else
		return false;
#endif
}
#endif

#if CM_HAVE_MSC
static CM_PopCountFn g_PopCountDispatch = CM_PopulationCountFallback;
static CM_BitDepFn g_BitDepDispatch = CM_BitDepositFallback;
static CM_BitExtFn g_BitExtDispatch = CM_BitExtractFallback;
static CM_BsfFn g_BsfDispatch = CM_BitScanForwardFallback;
static CM_PopLsbFn g_PopLsbDispatch = CM_PopLsbFallback;
#endif

void CM_DetectIntrinsics()
{
#if CM_HAVE_POPCNT || CM_HAVE_BMI2 ||CM_HAVE_BMI
	int cpuInfo[4]{};
#endif
#if CM_HAVE_POPCNT
	__cpuid(cpuInfo, 1);
#if CM_HAVE_MSC
	g_hasPOPCNT = (cpuInfo[2] & (1 << 23)) != 0;
	g_PopCountDispatch = g_hasPOPCNT ? CM_PopulationCountIntrinsic : CM_PopulationCountFallback;
#endif
#endif

#if CM_HAVE_BMI2
	__cpuid(cpuInfo, 7);
#if CM_HAVE_MSC
	g_hasBMI2 = (cpuInfo[1] & (1 << 8)) != 0;
	g_BitDepDispatch = g_hasBMI2 ? CM_BitDepositIntrinsic : CM_BitDepositFallback;
	g_BitExtDispatch = g_hasBMI2 ? CM_BitExtractIntrinsic : CM_BitExtractFallback;
#endif
#endif

#if CM_HAVE_BMI
	__cpuid(cpuInfo, 7);
#if CM_HAVE_MSC
	g_hasBMI = (cpuInfo[1] & (1 << 3)) != 0;
	g_BsfDispatch = g_hasBMI ? CM_BitScanForwardIntrinsic : CM_BitScanForwardFallback;
	g_PopLsbDispatch = g_hasBMI ? CM_PopLsbIntrinsic : CM_PopLsbFallback;
#endif
#endif
}

bool CM_HasPOPCNT()
{
#if CM_HAVE_POPCNT
#if CM_HAVE_MSC
	return g_hasPOPCNT.load();
#else
	return true;
#endif
#else
	return false;
#endif
}

bool CM_HasBMI2()
{
#if CM_HAVE_BMI2
#if CM_HAVE_MSC
	return g_hasBMI2.load();
#else
	return true;
#endif
#else
	return false;
#endif
}

bool CM_HasBMI()
{
#if CM_HAVE_BMI
#if CM_HAVE_MSC
	return g_hasBMI.load();
#else
	return true;
#endif
#else
	return false;
#endif
}

std::string CM_GetIntrinsicInfo()
{
	std::string result = "Runtime CPU features:";
	result += CM_HasPOPCNT() ? " POPCNT" : "";
	result += CM_HasBMI() ? " BMI" : "";
	result += CM_HasBMI2() ? " BMI2" : "";
	if (result == "Runtime CPU features:")
		result += " (none)";
	return result;
}

std::int8_t CM_PopulationCount(std::uint64_t value)
{
#if CM_HAVE_MSC
	return g_PopCountDispatch(value);
#else
#if CM_HAVE_POPCNT
	return CM_PopulationCountIntrinsic(value);
#else
	return CM_PopulationCountFallback(value);
#endif
#endif
}

std::uint64_t CM_BitDeposit(std::uint64_t value, std::uint64_t mask)
{
#if CM_HAVE_MSC
	return g_BitDepDispatch(value, mask);
#else
#if CM_HAVE_BMI2
	return CM_BitDepositIntrinsic(value, mask);
#else
	return CM_BitDepositFallback(value, mask);
#endif
#endif
}

std::uint64_t CM_BitExtract(std::uint64_t value, std::uint64_t mask)
{
#if CM_HAVE_MSC
	return g_BitExtDispatch(value, mask);
#else
#if CM_HAVE_BMI2
	return CM_BitExtractIntrinsic(value, mask);
#else
	return CM_BitExtractFallback(value, mask);
#endif
#endif
}

std::int8_t CM_BitScanForward(std::uint64_t value)
{
#if CM_HAVE_MSC
	return g_BsfDispatch(value);
#else
#if CM_HAVE_POPCNT
	return CM_BitScanForwardIntrinsic(value);
#else
	return CM_BitScanForwardFallback(value);
#endif
#endif
}

bool CM_PopLsb(std::uint64_t& value, std::int8_t& bit)
{
#if CM_HAVE_MSC
	return g_PopLsbDispatch(value, bit);
#else
#if CM_HAVE_POPCNT
	return CM_PopLsbIntrinsic(value, bit);
#else
	return CM_PopLsbFallback(value, bit);
#endif
#endif
}
