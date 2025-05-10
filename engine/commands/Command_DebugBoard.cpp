#include "Command_DebugBoard.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

Command_DebugBoard::Command_DebugBoard(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugBoard::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-board")
	{
		std::stringstream sstream;
		sstream << PositionToString(GetEngine().Position());
		if (GetEngine().Position().Hash != POSITION_ComputeHash(&GetEngine().Position()))
		{
			sstream << "Position hash does NOT match!" << std::endl;
		}
		GetEngine().OutputStream() << sstream.str();
		return true;
	}
	return false;
}
