#include "Command_DebugIntrinsics.h"
#include "../StringHelper.h"
#include "libCommon.h"
#include <iostream>

Command_DebugIntrinsics::Command_DebugIntrinsics(Engine* pEngine) :
	Command(pEngine)
{
}

bool Command_DebugIntrinsics::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) != "debug-intrinsics")
	{
		return false;
	}

	std::cout << CM_GetIntrinsicInfo() << std::endl;
	return true;
}