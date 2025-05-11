#pragma once
#ifndef BB_BITBOARD_H
#define BB_BITBOARD_H

#include <cstdint>
#include <ostream>

typedef std::uint64_t BB_BITBOARD;
typedef std::int8_t BB_SQUARECOUNT;


#define BITBOARD_UP(_BB_)			(_BB_<<8)
#define BITBOARD_DOWN(_BB_)			(_BB_>>8)
#define BITBOARD_LEFT(_BB_)			((_BB_>>1)&UINT64_C(0x7F7F7F7F7F7F7F7F))
#define BITBOARD_RIGHT(_BB_)		((_BB_<<1)&UINT64_C(0xFEFEFEFEFEFEFEFE))
#define BITBOARD_UPLEFT(_BB_)		BITBOARD_UP(BITBOARD_LEFT(_BB_))
#define BITBOARD_UPRIGHT(_BB_)		BITBOARD_UP(BITBOARD_RIGHT(_BB_))
#define BITBOARD_DOWNLEFT(_BB_)		BITBOARD_DOWN(BITBOARD_LEFT(_BB_))
#define BITBOARD_DOWNRIGHT(_BB_)	BITBOARD_DOWN(BITBOARD_RIGHT(_BB_))
#define BITBOARD_EMPTY				UINT64_C(0)

BB_SQUARECOUNT BITBOARD_PopulationCount(const BB_BITBOARD& bitboard);

BB_BITBOARD BITBOARD_BitDeposit(const BB_BITBOARD& bitboard, const BB_BITBOARD& mask);

BB_BITBOARD BITBOARD_BitExtract(const BB_BITBOARD& bitboard, const BB_BITBOARD& mask);

void BITBOARD_Dump(const BB_BITBOARD& bitboard, const char* pStr = nullptr);


#endif