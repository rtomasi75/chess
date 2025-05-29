#include "SE_NODESTATE.h"
#include "libCommon.h"
#include <cstring>

bool NODESTATE_ToString(char* pString, int len, int& strPos, SE_NODESTATE state)
{
	int tmpPos = strPos;
	const char* name = nullptr;
	switch (state)
	{
	case NODESTATE_GENERATE:
		name = "GENERATE";
		break;
	case NODESTATE_FORK:   
		name = "FORK";   
		break;
	case NODESTATE_ITERATE: 
		name = "ITERATE";
		break;
	case NODESTATE_APPLY:  
		name = "APPLY";  
		break;
	case NODESTATE_PROCESS: 
		name = "PROCESS"; 
		break;
	case NODESTATE_EXPLORE:
		name = "EXPLORE"; 
		break;
	case NODESTATE_RESUME:
		name = "RESUME";   
		break;
	case NODESTATE_JOIN:  
		name = "JOIN";  
		break;
	case NODESTATE_FINISH:  
		name = "FINISH"; 
		break;
	case NODESTATE_INVALID: 
		name = "INVALID";
		break;
	default:                 
		ASSERT(false);
		break;
	}
	const size_t strLen = std::strlen(name);
	if ((strPos + strLen) >= len)
		return false;
	memcpy(pString + strPos, name, strLen);
	strPos += strLen;
	return true;
}