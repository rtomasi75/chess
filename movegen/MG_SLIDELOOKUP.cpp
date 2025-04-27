#include "MG_SLIDELOOKUP.h"
#include "MG_MOVEGEN.h"

size_t SLIDELOOKUP_CountSlideEntries(const MG_MOVEGEN* pMoveGen)
{
	size_t count = 0;
	for (int lookUpIndex = 0; lookUpIndex < COUNT_SLIDELOOKUPS; lookUpIndex++)
	{
		for (MG_SLIDEMASKINDEX maskIndex = 0; maskIndex < pMoveGen->SlideLookUp[lookUpIndex].CountMasks; maskIndex++)
		{
			for (BB_SQUAREINDEX squareIndex = 0; squareIndex < COUNT_SQUARES; squareIndex++)
			{
				BB_SQUARECOUNT population = BITBOARD_PopulationCount(pMoveGen->SlideMasks[maskIndex].Mask[squareIndex]);
				count += (((size_t)1) << population);
			}
		}
	}
	return count;
}
