#pragma once
#ifndef MG_SLIDEMASKS_H
#define MG_SLIDEMASKS_H

#include "libBitboard.h"

typedef std::int8_t MG_SLIDEMASKINDEX;

struct MG_SLIDEMASKS
{
	BB_BITBOARD Mask[COUNT_SQUARES];
};

struct MG_MOVEGEN;

#define COUNT_SLIDEMASKS 2
#define SLIDEMASKS_HORIZONTAL 0
#define SLIDEMASKS_DIAGONAL 1

BB_BITBOARD SLIDEMASKS_GenerateMaskHorizontal(const BB_SQUARE& square);

BB_BITBOARD SLIDEMASKS_GenerateMaskDiagonal(const BB_SQUARE& square);

void SLIDEMASKS_Initialize(MG_MOVEGEN* pMoveGen);

#endif