#include "BB_FILE.h"
#include "libCommon.h"

bool FILE_Parse(const char* pString, const int& len, int& strPos, BB_FILE& outParsed)
{
	if (strPos >= len)
		return false;
	switch (pString[strPos])
	{
	default:
		return false;
	case 'a':
	case 'A':
		outParsed = FILE_A;
		break;
	case 'b':
	case 'B':
		outParsed = FILE_B;
		break;
	case 'c':
	case 'C':
		outParsed = FILE_C;
		break;
	case 'd':
	case 'D':
		outParsed = FILE_D;
		break;
	case 'e':
	case 'E':
		outParsed = FILE_E;
		break;
	case 'f':
	case 'F':
		outParsed = FILE_F;
		break;
	case 'g':
	case 'G':
		outParsed = FILE_G;
		break;
	case 'h':
	case 'H':
		outParsed = FILE_H;
		break;
	}
	strPos++;
	return true;
}

bool FILE_ToString(char* pString, const int& len, int& strPos, const BB_FILE& file)
{
	if (strPos >= len)
		return false;
	switch (file)
	{
	default:
		ASSERT(false);
		break;
	case FILE_A:
		pString[strPos] = 'a';
		break;
	case FILE_B:
		pString[strPos] = 'b';
		break;
	case FILE_C:
		pString[strPos] = 'c';
		break;
	case FILE_D:
		pString[strPos] = 'd';
		break;
	case FILE_E:
		pString[strPos] = 'e';
		break;
	case FILE_F:
		pString[strPos] = 'f';
		break;
	case FILE_G:
		pString[strPos] = 'g';
		break;
	case FILE_H:
		pString[strPos] = 'h';
		break;
	}
	strPos++;
	return true;
}

