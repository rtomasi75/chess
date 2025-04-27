#include "Command_DebugMove.h"
#include "../StringHelper.h"
#include "../Engine.h"

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
				GetEngine().OutputStream() << "Performing move " + MoveToString(move) + ":" << std::endl;
				GetEngine().MakeMove(move);
				GetEngine().OutputStream() << PositionToString(GetEngine().Position());
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
