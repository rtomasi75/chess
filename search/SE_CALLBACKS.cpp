#include "SE_CALLBACKS.h"
#include "libCommon.h"
#include <cstring>

void CALLBACKS_Initialize(SE_CALLBACKS* pCallbacks, SE_FN_ONFSMCOMPLETE onFsmComplete)
{
    ASSERT(onFsmComplete != nullptr);             
    memset(pCallbacks, 0, sizeof(SE_CALLBACKS));
    pCallbacks->OnFsmComplete = onFsmComplete;
}