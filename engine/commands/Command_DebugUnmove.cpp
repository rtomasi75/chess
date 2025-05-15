#include "Command_DebugUnmove.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

Command_DebugUnmove::Command_DebugUnmove(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugUnmove::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-unmove")
	{
		if (GetEngine().Game().CurrentMove > 0)
		{
			std::stringstream sstream;
			sstream << "Undoing move " + MoveToString(GetEngine().Game().PlayedMoves[GetEngine().Game().CurrentMove - 1].Move) + ":" << std::endl;
			GetEngine().UnmakeMove();
			sstream << PositionToString(GetEngine().Position());
			if (GetEngine().Position().Hash != POSITION_ComputeHash(&GetEngine().Position()))
			{
				sstream << "Position hash does NOT match!" << std::endl;
			}
			GetEngine().OutputStream() << sstream.str();
			return true;
		}
		else
		{
			GetEngine().ErrorMessage("No moves have been played yet.");
			return true;
		}
	}
	return false;
}
