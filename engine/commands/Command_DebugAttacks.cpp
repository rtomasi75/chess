#include "Command_DebugAttacks.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>

Command_DebugAttacks::Command_DebugAttacks(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugAttacks::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "debug-attacks")
		{
			std::string pieceString = remainder;
			int pos = 0;
			MG_PLAYER player;
			MG_PIECETYPE piece;
			if (!PIECETYPE_Parse(pieceString.c_str(), static_cast<int>(pieceString.length()), pos, piece, player))
			{
				GetEngine().ErrorMessage(pieceString + " is not a valid piece/player notation (failed to parse).");
				return true;
			}
			std::stringstream sstream;
			BB_BITBOARD interest;
			sstream << "Attack map for " << PieceToString(piece, player) << std::endl;
			const MG_POSITION position = GetEngine().Position();
			sstream << BitboardToString(MOVEGEN_GetPieceAttacks(&GetEngine().MoveGen(), &position, piece, player, interest), 2) << std::endl;
			sstream << "Interest map for " << PieceToString(piece, player) << std::endl;
			sstream << BitboardToString(interest, 2) << std::endl;
			GetEngine().OutputStream() << sstream.str();
			return true;
		}
	}
	return false;
}
