#include "Command_Quit.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_Quit::Command_Quit(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_Quit::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "quit")
	{
		GetEngine().Stop();
		return true;
	}
	return false;
}
