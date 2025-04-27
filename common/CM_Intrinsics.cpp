#include "CM_Intrinsics.h"


const std::int8_t IndicesBSF[64] =
{
	0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

std::int8_t CM_BitScanForward(const std::uint64_t& bb)
{
	const std::uint64_t debruijn64 = UINT64_C(0x03f79d71b4cb0a89);
	return (int)IndicesBSF[((bb & (std::uint64_t)(-(std::int64_t)bb)) * debruijn64) >> 58];
}

std::int8_t CM_PopulationCount(const std::uint64_t& bb)
{
	std::uint64_t count = (bb & UINT64_C(0x5555555555555555)) + ((bb & UINT64_C(0xaaaaaaaaaaaaaaaa)) >> 1);
	count = (count & UINT64_C(0x3333333333333333)) + ((count & UINT64_C(0xcccccccccccccccc)) >> 2);
	count = (count & UINT64_C(0x0f0f0f0f0f0f0f0f)) + ((count & UINT64_C(0xf0f0f0f0f0f0f0f0)) >> 4);
	count = (count & UINT64_C(0x00ff00ff00ff00ff)) + ((count & UINT64_C(0xff00ff00ff00ff00)) >> 8);
	count = (count & UINT64_C(0x0000ffff0000ffff)) + ((count & UINT64_C(0xffff0000ffff0000)) >> 16);
	count = (count & UINT64_C(0x00000000ffffffff)) + ((count & UINT64_C(0xffffffff00000000)) >> 32);
	return (std::int8_t)count;
}

std::uint64_t CM_BitDeposit(const std::uint64_t& val, std::uint64_t mask)
{
	std::uint64_t res = UINT64_C(0);
	for (std::uint64_t bb = UINT64_C(1); mask; bb += bb)
	{
		if (val & bb)
		{
			res |= mask & (std::uint64_t)(-(std::int64_t)mask);
		}
		mask &= mask - 1;
	}
	return res;
}

std::uint64_t CM_BitExtract(const std::uint64_t& val, std::uint64_t mask)
{
	std::uint64_t res = UINT64_C(0);
	for (std::uint64_t bb = UINT64_C(1); mask; bb += bb)
	{
		if (val & mask & (std::uint64_t)(-(std::int64_t)mask))
		{
			res |= bb;
		}
		mask &= mask - 1;
	}
	return res;
}