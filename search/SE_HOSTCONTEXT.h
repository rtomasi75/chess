#pragma once
#ifndef SE_HOSTCONTEXT_H
#define SE_HOSTCONTEXT_H

#include "SE_Types.h"
#include "SE_CALLBACKS.h"
#include "SE_FSM.h"

struct SE_HOSTCONTEXT
{
	const SE_CALLBACKS* pCallbacks;
	SE_CONTEXTPTR pSearchContext;
	SE_EXECUTIONTOKEN Token;
};

void HOSTCONTEXT_Initialize(SE_HOSTCONTEXT* pHostContext, const SE_CALLBACKS* pCallbacks, SE_CONTEXTPTR pSearchContext, SE_EXECUTIONTOKEN token);

#endif // SE_HOSTCONTEXT_H