#include "MG_PLAYER.h"
#include "libCommon.h"

bool PLAYER_Parse(const char* pString, const int& len, int& strPos, MG_PLAYER& outParsedPlayer)
{
	if (strPos >= len)
		return false;
	switch (pString[strPos])
	{
	default:
		return false;
	case 'b':
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'w':
		outParsedPlayer = PLAYER_WHITE;
		break;
	}
	strPos++;
	return true;
}

bool PLAYER_ToString(char* pString, const int& len, int& strPos, const MG_PLAYER& player)
{
	if (strPos >= len)
		return false;
	switch (player)
	{
	default:
		ASSERT(false);
		break;
	case PLAYER_WHITE:
		pString[strPos] = 'w';
		break;
	case PLAYER_BLACK:
		pString[strPos] = 'b';
		break;
	}
	strPos++;
	return true;
}
