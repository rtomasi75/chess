#pragma once
#ifndef SE_CALLBACKS_H
#define SE_CALLBACKS_H

#include "SE_Types.h"

typedef void (*SE_FN_ONFSMCOMPLETE)(SE_EXECUTIONTOKEN);

struct SE_CALLBACKS
{
	SE_FN_ONFSMCOMPLETE OnFsmComplete;
};

void CALLBACKS_Initialize(SE_CALLBACKS* pCallbacks, SE_FN_ONFSMCOMPLETE onFsmComplete);

#endif