#pragma once
#ifndef CM_HELPERS_H
#define CM_HELPERS_H

bool CM_ParseDecimalInt(const char* pString, const int len, int& strPos, int& outValue);

void CM_SetCurrentThreadName(const char* name);

#define CM_YIELD() std::this_thread::yield()

#define CM_SLEEP(_mcs_) std::this_thread::sleep_for(std::chrono::microseconds(_mcs_))

#endif