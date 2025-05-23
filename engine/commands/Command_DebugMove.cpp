#include "Command_DebugMove.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

Command_DebugMove::Command_DebugMove(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugMove::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "debug-move")
		{
			MG_MOVE move;
			if (ParseMove(remainder, move))
			{
				std::stringstream sstream;
				sstream << "Performing move " + MoveToString(move) + ":" << std::endl;
				GetEngine().MakeMove(move);
				sstream << PositionToString(GetEngine().Position());
				MG_POSITION position = GetEngine().Position();
				if (position.Hash != POSITION_ComputeHash(&position))
				{
					sstream << "Position hash does NOT match!" << std::endl;
				}
				GetEngine().OutputStream() << sstream.str();
				return true;
			}
			else
			{
				GetEngine().ErrorMessage(remainder + " is not a legal move in the current position.");
				return true;
			}
		}
	}
	return false;
}
