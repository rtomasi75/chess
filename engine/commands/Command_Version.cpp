#include "Command_Version.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_Version::Command_Version(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_Version::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "version")
	{
		if (GetEngine().IsUCI())
		{
			GetEngine().OutputStream() << "info string " << GetEngine().Version() << std::endl;
		}
		else
		{
			GetEngine().OutputStream() << GetEngine().Version() << std::endl;
		}
		return true;
	}
	return false;
}
