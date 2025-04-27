#include "Command.h"
#include "StringHelper.h"
#include "Engine.h"
#include <sstream>

Command::Command(Engine* pEngine) :
	_pEngine(pEngine)
{

}

Engine& Command::GetEngine()
{
	return *_pEngine;
}

const Engine& Command::GetEngine() const
{
	return *_pEngine;
}

std::string Command::PieceToString(const MG_PIECETYPE& piece, const MG_PLAYER& player) const
{
	char buffer[2];
	memset(buffer, 0, 2);
	int pos = 0;
	PIECETYPE_ToString(buffer, 2, pos, piece, player);
	return std::string(buffer);
}

std::string Command::BitboardToString(const BB_BITBOARD& bitboard, int indentation) const
{
	std::stringstream sstream;
	for (BB_RANKINDEX rankIndex = COUNT_RANKS - 1; rankIndex >= 0; rankIndex--)
	{
		for (int indent = 0; indent < indentation; indent++)
			sstream << " ";
		for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
		{
			BB_SQUARE square = SQUARE_FromRankFileIndices(rankIndex, fileIndex);
			if (bitboard & square)
				sstream << "#";
			else
				sstream << ".";
		}
		sstream << std::endl;
	}
	return sstream.str();
}

bool Command::ParseMove(const std::string& moveString, MG_MOVE& parsedMove) const
{
	std::string temp = moveString;
	std::string normalized = StringHelper::ToLower(StringHelper::Trim(temp));
	int pos = 0;
	return MOVEGEN_ParseMoveString(&GetEngine().MoveGen(), GetEngine().Position().MovingPlayer, &GetEngine().LegalMoves(), moveString.c_str(), (int)moveString.size(), pos, parsedMove);
}

bool Command::ParseSquare(const std::string& squareString, BB_SQUARE& parsedSquare) const
{
	std::string temp = squareString;
	std::string normalized = StringHelper::ToLower(StringHelper::Trim(temp));
	int pos = 0;
	return SQUARE_Parse(squareString.c_str(), (int)squareString.size(), pos, parsedSquare);
}

std::string Command::MoveToString(const MG_MOVE& move) const
{
	return std::string(GetEngine().MoveGen().MoveTable[GetEngine().Position().MovingPlayer][move].MoveString);
}

std::string Command::MoveToHexString(const MG_MOVE& move) const
{
	return StringHelper::ToHexString(move);
}

std::string Command::SquareToString(const BB_SQUARE& square) const
{
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	SQUARE_ToString(buffer, 16, pos, square);
	return buffer;
}

std::string Command::PositionToString(const MG_POSITION& position, int indentation) const
{
	std::stringstream sstream;
	for (BB_RANKINDEX rankIndex = COUNT_RANKS - 1; rankIndex >= 0; rankIndex--)
	{
		for (int i = 0; i < indentation; i++)
		{
			sstream << " ";
		}
		BB_RANK rank = RANK_FromIndex(rankIndex);
		for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
		{
			BB_FILE file = FILE_FromIndex(fileIndex);
			BB_SQUARE sq = rank & file;
			if (POSITION_CheckConsistency(&GetEngine().Position(), sq))
			{
				MG_PIECETYPE piece;
				MG_PLAYER player;
				if (POSITION_GetPiece(&GetEngine().Position(), sq, player, piece))
				{
					sstream << PieceToString(piece, player);
				}
				else
				{
					sstream << ".";
				}
			}
			else
				sstream << "E";
		}
		sstream << std::endl;
	}
	return sstream.str();
}
