#include "SE_HOSTCONTEXT.h"
#include "libCommon.h"

void HOSTCONTEXT_Initialize(SE_HOSTCONTEXT* pHostContext, const SE_CALLBACKS* pCallbacks, SE_CONTEXTPTR pSearchContext, SE_EXECUTIONTOKEN token)
{
	ASSERT(pHostContext);
	ASSERT(pCallbacks);
	ASSERT(pSearchContext);
	ASSERT(token);
	memcpy(&pHostContext->Callbacks, pCallbacks, sizeof(SE_CALLBACKS));
	memcpy(&pHostContext->SearchContext, pSearchContext, sizeof(SE_SEARCHCONTEXTSTORAGE));
	pHostContext->Token = token;
}