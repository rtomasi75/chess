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
		const MG_POSITION position = GetEngine().Position();
		std::stringstream sstream;
		sstream << PositionToString(position);
		sstream << std::endl;
		for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
		{
			sstream << " " << PlayerToString(player) << "         ";
			for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
			{
				sstream << PieceToString(piece, player) << "         ";
			}
			sstream << std::endl;
			for (BB_RANKINDEX rankIdx = COUNT_RANKS - 1; rankIdx >= 0; rankIdx--)
			{
				sstream << " ";
				for (BB_FILEINDEX fileIdx = 0; fileIdx < COUNT_FILES; fileIdx++)
				{
					const BB_SQUARE square = SQUARE_FromRankFileIndices(rankIdx, fileIdx);
					if (square & position.AttacksPlayer[player])
					{
						sstream << "#";
					}
					else
					{
						sstream << ".";
					}
				}
				sstream << "  ";
				for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
				{
					for (BB_FILEINDEX fileIdx = 0; fileIdx < COUNT_FILES; fileIdx++)
					{
						const BB_SQUARE square = SQUARE_FromRankFileIndices(rankIdx, fileIdx);
						if (square & position.AttacksPlayerPiece[player][piece])
						{
							sstream << "#";
						}
						else
						{
							sstream << ".";
						}
					}
					sstream << "  ";
				}
				sstream << std::endl;
			}
			sstream << "           ";
			for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
			{
				sstream << "[" << PieceToString(piece, player) << "]       ";
			}
			sstream << std::endl;
			for (BB_RANKINDEX rankIdx = COUNT_RANKS - 1; rankIdx >= 0; rankIdx--)
			{
				sstream << " ";
				for (BB_FILEINDEX fileIdx = 0; fileIdx < COUNT_FILES; fileIdx++)
				{
					sstream << " ";
				}
				sstream << "  ";
				for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
				{
					for (BB_FILEINDEX fileIdx = 0; fileIdx < COUNT_FILES; fileIdx++)
					{
						const BB_SQUARE square = SQUARE_FromRankFileIndices(rankIdx, fileIdx);
						if (square & position.InterestPlayerPiece[player][piece])
						{
							sstream << "#";
						}
						else
						{
							sstream << ".";
						}
					}
					sstream << "  ";
				}
				sstream << std::endl;
			}
			sstream << std::endl;
		}
		if (position.Hash != POSITION_ComputeHash(&position))
		{
			sstream << "WARNING: Position hash does NOT match!" << std::endl;
		}
		GetEngine().OutputStream() << sstream.str();
		return true;
	}
	return false;
}
