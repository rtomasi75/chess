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
			const MG_MOVE move = GetEngine().Game().PlayedMoves[GetEngine().Game().CurrentMove - 1].Move;
			GetEngine().UnmakeMove();
			sstream << "Undoing move " + MoveToString(move) + ":" << std::endl;
			sstream << PositionToString(GetEngine().Position());
			const MG_POSITION position = GetEngine().Position();
			if (position.Hash != POSITION_ComputeHash(&position))
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
