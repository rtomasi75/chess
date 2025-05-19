#include "MG_MOVEINFO.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

void MOVEINFO_InitializeMoveString(MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVE& move, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare)
{
	int strPos;
	for (strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		(pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH)[strPos] = 0;
	}
	strPos = 0;
	bool result = SQUARE_ToString((pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH), MOVESTRING_LENGTH, strPos, fromSquare);
	result &= SQUARE_ToString((pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH), MOVESTRING_LENGTH, strPos, toSquare);
	ASSERT(result);
}

void MOVEINFO_InitializeMoveStringPromotion(MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVE& move, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare, const MG_PIECETYPE& promoPiece)
{
	int strPos;
	for (strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		(pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH)[strPos] = 0;
	}
	strPos = 0;
	bool result = SQUARE_ToString((pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH), MOVESTRING_LENGTH, strPos, fromSquare);
	result &= SQUARE_ToString((pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH), MOVESTRING_LENGTH, strPos, toSquare);
	result &= PIECETYPE_ToString((pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH), MOVESTRING_LENGTH, strPos, promoPiece, PLAYER_BLACK);
	ASSERT(result);
}

