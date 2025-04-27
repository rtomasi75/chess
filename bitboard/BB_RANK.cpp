#include "BB_RANK.h"
#include "libCommon.h"

BB_RANK RANK_FromIndex(const BB_RANKINDEX& index)
{
	return UINT64_C(0x00000000000000FF) << (8 * index);
}

bool RANK_Parse(const char* pString, const int& len, int& strPos, BB_RANK& outParsed)
{
	if (strPos >= len)
		return false;
	switch (pString[strPos])
	{
	default:
		return false;
	case '1':
		outParsed = RANK_1;
		break;
	case '2':
		outParsed = RANK_2;
		break;
	case '3':
		outParsed = RANK_3;
		break;
	case '4':
		outParsed = RANK_4;
		break;
	case '5':
		outParsed = RANK_5;
		break;
	case '6':
		outParsed = RANK_6;
		break;
	case '7':
		outParsed = RANK_7;
		break;
	case '8':
		outParsed = RANK_8;
		break;
	}
	strPos++;
	return true;
}

bool RANK_ToString(char* pString, const int& len, int& strPos, const BB_RANK& rank)
{
	if (strPos >= len)
		return false;
	switch (rank)
	{
	default:
		ASSERT(false);
		break;
	case RANK_1:
		pString[strPos] = '1';
		break;
	case RANK_2:
		pString[strPos] = '2';
		break;
	case RANK_3:
		pString[strPos] = '3';
		break;
	case RANK_4:
		pString[strPos] = '4';
		break;
	case RANK_5:
		pString[strPos] = '5';
		break;
	case RANK_6:
		pString[strPos] = '6';
		break;
	case RANK_7:
		pString[strPos] = '7';
		break;
	case RANK_8:
		pString[strPos] = '8';
		break;
	}
	strPos++;
	return true;
}
