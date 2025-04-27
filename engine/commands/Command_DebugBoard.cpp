#include "Command_DebugBoard.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_DebugBoard::Command_DebugBoard(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugBoard::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-board")
	{
		GetEngine().OutputStream() << PositionToString(GetEngine().Position());
		return true;
	}
	return false;
}
