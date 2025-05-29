#include "CM_Helpers.h"
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#elif defined __APPLE__
#include <pthread.h>
#elif defined __linux__
#include <pthread.h>
#endif
#include <string>

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

void CM_SetCurrentThreadName(const char* name)
{
#if defined(_WIN32)
	SetThreadDescription(GetCurrentThread(), std::wstring(name, name + strlen(name)).c_str());
#elif defined(__APPLE__)
	pthread_setname_np(name); // macOS
#elif defined(__linux__)
	pthread_setname_np(pthread_self(), name); // Linux
#endif
}
