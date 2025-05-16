#include "Command_DebugIntrinsics.h"
#include "../StringHelper.h"
#include "libCommon.h"
#include <iostream>
#include <sstream>
#include "../Engine.h"

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
	std::stringstream sstream;
	sstream << CM_GetIntrinsicInfo() << std::endl;
	sstream << "CM_ALIGNMENT: " << CM_ALIGNMENT << " bytes" << std::endl;
	sstream << "MG_POSITION alignment: " << alignof(MG_POSITION) << " bytes" << std::endl;
	GetEngine().OutputStream() << sstream.str();
	return true;
}