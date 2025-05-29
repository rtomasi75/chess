#pragma once
#ifndef SE_MOVEMASK_H
#define SE_MOVEMASK_H

#include <cstdint>
#include "libMovegen.h"

typedef std::uint64_t SE_FORKSUBMASK;

#define SE_COUNT_FORKSUBMASKS (1 +(MAX_MOVES / 64))

struct SE_FORKMASK
{
	SE_FORKSUBMASK SubMask[SE_COUNT_FORKSUBMASKS];
};

void FORKMASK_Initialize(SE_FORKMASK* pForkMask);

bool FORKMASK_CheckIfMasked(const SE_FORKMASK* pForkMask, const MG_MOVEINDEX moveIndex);

void FORKMASK_MaskMove(SE_FORKMASK* pForkMask, const MG_MOVEINDEX moveIndex);


#endif // SE_MOVEMASK_H