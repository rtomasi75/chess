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

void MOVEINFO_InitializeMoveStringPromotion(char* pMoveString, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare, const MG_PIECETYPE& promoPiece)
{
	int strPos;
	for (strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		pMoveString[strPos] = 0;
	}
	strPos = 0;
	bool result = SQUARE_ToString(pMoveString, MOVESTRING_LENGTH, strPos, fromSquare);
	result &= SQUARE_ToString(pMoveString, MOVESTRING_LENGTH, strPos, toSquare);
	result &= PIECETYPE_ToString(pMoveString, MOVESTRING_LENGTH, strPos, promoPiece, PLAYER_BLACK);
	ASSERT(result);
}

