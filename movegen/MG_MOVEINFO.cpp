#include "MG_MOVEINFO.h"
#include "libCommon.h"

void MOVEINFO_InitializeMoveString(char* pMoveString, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare)
{
	int strPos;
	for (strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		pMoveString[strPos] = 0;
	}
	strPos = 0;
	bool result = SQUARE_ToString(pMoveString, MOVESTRING_LENGTH, strPos, fromSquare);
	result &= SQUARE_ToString(pMoveString, MOVESTRING_LENGTH, strPos, toSquare);
	ASSERT(result);
}

BB_BITBOARD MOVEINFO_GetMoveMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->MoveMap;
#else
	return SQUARE_FromIndex(pMoveInfo->MoveDest) ^ SQUARE_FromIndex(pMoveInfo->MoveSource);
#endif
}

BB_BITBOARD MOVEINFO_GetKillMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->KillMap;
#else
	return SQUARE_FromIndex(pMoveInfo->KillDest);
#endif
}

BB_BITBOARD MOVEINFO_GetCreateMap(const MG_MOVEINFO* pMoveInfo)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	return pMoveInfo->CreateMap;
#else
	return SQUARE_FromIndex(pMoveInfo->CreateDest);
#endif
}
