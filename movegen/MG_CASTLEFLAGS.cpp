#include "MG_CASTLEFLAGS.h"
#include "libCommon.h"


bool CASTLEFLAGS_Parse(const char* pString, const int& len, int& strPos, MG_CASTLEFLAGS& outCastleFlags)
{
	if (strPos >= len)
		return false;
	outCastleFlags = CASTLEFLAGS_NONE;
	if (pString[strPos] == '-')
	{
		strPos++;
		return true;
	}
	int idx;
	for (idx = 0; idx < 4; idx++)
	{
		bool bBreakOut = false;
		if (strPos >= len)
			break;
		switch (pString[strPos++])
		{
		default:
			bBreakOut = true;
			break;
		case 'K':
			outCastleFlags |= CASTLEFLAGS_WHITE_KINGSIDE;
			break;
		case 'Q':
			outCastleFlags |= CASTLEFLAGS_WHITE_QUEENSIDE;
			break;
		case 'k':
			outCastleFlags |= CASTLEFLAGS_BLACK_KINGSIDE;
			break;
		case 'q':
			outCastleFlags |= CASTLEFLAGS_BLACK_QUEENSIDE;
			break;
		}
		if (bBreakOut)
			break;
	}
	return idx > 0;
}

bool CASTLEFLAGS_ToString(char* pString, const int& len, int& strPos, const MG_CASTLEFLAGS& castleFlags)
{
	if (strPos >= len)
		return false;
	if (castleFlags == CASTLEFLAGS_NONE)
	{
		pString[strPos++] = '-';
		return true;
	}
	if (castleFlags & CASTLEFLAGS_WHITE_KINGSIDE)
	{
		pString[strPos++] = 'K';
	}
	if (castleFlags & CASTLEFLAGS_WHITE_QUEENSIDE)
	{
		pString[strPos++] = 'Q';
	}
	if (castleFlags & CASTLEFLAGS_BLACK_KINGSIDE)
	{
		pString[strPos++] = 'k';
	}
	if (castleFlags & CASTLEFLAGS_BLACK_QUEENSIDE)
	{
		pString[strPos++] = 'q';
	}
	return true;
}
