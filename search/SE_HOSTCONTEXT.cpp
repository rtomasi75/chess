#include "SE_HOSTCONTEXT.h"
#include "libCommon.h"

void HOSTCONTEXT_Initialize(SE_HOSTCONTEXT* pHostContext, const SE_CALLBACKS* pCallbacks, SE_CONTEXTPTR pSearchContext, SE_EXECUTIONTOKEN token)
{
	ASSERT(pHostContext);
	ASSERT(pCallbacks);
	ASSERT(pSearchContext);
	ASSERT(token);
	pHostContext->pCallbacks = pCallbacks;
	pHostContext->pSearchContext = pSearchContext;
	pHostContext->Token = token;
}