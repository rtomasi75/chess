#pragma once
#ifndef SE_NODESTATE_H
#define SE_NODESTATE_H

#include <cstdint>

typedef std::int8_t SE_NODESTATE;

#define NODESTATE_GENERATE INT8_C(0)
#define NODESTATE_ITERATE INT8_C(1)
#define NODESTATE_RESUME INT8_C(2)
#define NODESTATE_PROCESS INT8_C(3)
#define NODESTATE_APPLY INT8_C(4)
#define NODESTATE_EXPLORE INT8_C(5)
#define NODESTATE_FORK INT8_C(6)
#define NODESTATE_JOIN INT8_C(7)
#define NODESTATE_FINISH INT8_C(8)
#define NODESTATE_INVALID INT8_C(-1)

bool NODESTATE_ToString(char* pString, int len, int& strPos, SE_NODESTATE state);

#endif // SE_NODESTATE_H