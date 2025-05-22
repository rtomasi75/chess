#include "CM_Helpers.h"
#include <limits.h>

bool CM_ParseDecimalInt(const char* pString, const int len, int& strPos, int& outValue)
{
	int value = 0;
	bool parsed = false;
	while (strPos < len)
	{
		const char c = pString[strPos];
		if (c >= '0' && c <= '9')
		{
			const int digit = c - '0';
			if (value > (INT_MAX - digit) / 10)
				return false;
			value = value * 10 + digit;
			strPos++;
			parsed = true;
		}
		else
		{
			break;
		}
	}
	if (!parsed)
		return false;
	outValue = value;
	return true;
}