#include "Command_SetFen.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>
#include <chrono>

Command_SetFen::Command_SetFen(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_SetFen::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "set-fen")
		{
			if (StringHelper::Trim(remainder) != "")
			{
				std::stringstream sstream;
				MG_POSITION position;
				if (PositionFromFEN(position, remainder))
				{
					GetEngine().SetPosition(position);
					sstream << "New Position: " << std::endl;
					sstream << PositionToString(GetEngine().Position());
				}
				else
					sstream << "ERROR: '" << remainder << "' is not a valid FEN string." << std::endl;
				GetEngine().OutputStream() << sstream.str();
				return true;
			}
		}
	}
	return false;
}
