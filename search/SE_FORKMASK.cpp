#include "SE_FORKMASK.h"
#include "libMovegen.h"

void FORKMASK_Initialize(SE_FORKMASK* pForkMask)
{
	for (MG_MOVEINDEX subMaskIndex = 0; subMaskIndex < SE_COUNT_FORKSUBMASKS; subMaskIndex++)
	{
		pForkMask->SubMask[subMaskIndex] = UINT64_C(0);
	}
}

bool FORKMASK_CheckIfMasked(const SE_FORKMASK* pForkMask, const MG_MOVEINDEX moveIndex)
{
	const MG_MOVEINDEX subMaskIndex = moveIndex / 64;
	const MG_MOVEINDEX subMaskBitIndex = moveIndex % 64;
	return (pForkMask->SubMask[subMaskIndex] & (UINT64_C(1) << subMaskBitIndex)) != 0;
}

void FORKMASK_MaskMove(SE_FORKMASK* pForkMask, const MG_MOVEINDEX moveIndex)
{
	const MG_MOVEINDEX subMaskIndex = moveIndex / 64;
	const MG_MOVEINDEX subMaskBitIndex = moveIndex % 64;
	pForkMask->SubMask[subMaskIndex] |= (UINT64_C(1) << subMaskBitIndex);
}