#pragma once
#ifndef CM_HELPERS_H
#define CM_HELPERS_H

bool CM_ParseDecimalInt(const char* pString, const int len, int& strPos, int& outValue);

void CM_SetCurrentThreadName(const char* name);

#endif