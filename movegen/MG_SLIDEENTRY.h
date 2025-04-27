#pragma once
#ifndef MG_SLIDEENTRY_H
#define MG_SLIDEENTRY_H

#include "libBitboard.h"

struct MG_SLIDEENTRY
{
	BB_BITBOARD TargetsFrom[COUNT_SQUARES];
};

#endif