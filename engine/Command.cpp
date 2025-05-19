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
	return MOVEGEN_ParseMoveString(&GetEngine().MoveGen(), GetEngine().Position().Header.MovingPlayer, &GetEngine().LegalMoves(), moveString.c_str(), (int)moveString.size(), pos, parsedMove);
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
	return std::string(MOVEGEN_GetMoveString(&GetEngine().MoveGen(), GetEngine().Position().Header.MovingPlayer, move));
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

std::string Command::RankToString(const BB_RANK& rank) const
{
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	RANK_ToString(buffer, 16, pos, rank);
	return buffer;
}

std::string Command::FileToString(const BB_FILE& file) const
{
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	FILE_ToString(buffer, 16, pos, file);
	return buffer;
}

std::string Command::CastleFlagsToString(const MG_CASTLEFLAGS& castleFlags) const
{
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	CASTLEFLAGS_ToString(buffer, 16, pos, castleFlags);
	return buffer;
}

std::string Command::PlayerToString(const MG_PLAYER& player) const
{
	char buffer[16];
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	PLAYER_ToString(buffer, 16, pos, player);
	return buffer;
}

std::string Command::PositionToFEN(const MG_POSITION& position) const
{
	char buffer[256];
	for (int i = 0; i < 256; i++)
	{
		buffer[i] = 0;
	}
	int pos = 0;
	POSITION_ToString(buffer, 256, pos, position);
	return buffer;
}

bool Command::PositionFromFEN(MG_POSITION& position, const std::string& fen) const
{
	int strPos = 0;
	return POSITION_Parse(&GetEngine().MoveGen(), fen.c_str(), static_cast<int>(fen.size()), strPos, position);
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
		sstream << RankToString(rank) << "|";
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
	for (int i = 0; i < indentation; i++)
	{
		sstream << " ";
	}
	sstream << "-+";
	for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
	{
		sstream << "-";
	}
	sstream << std::endl;
	for (int i = 0; i < indentation; i++)
	{
		sstream << " ";
	}
	sstream << PlayerToString(GetEngine().Position().Header.MovingPlayer);
	sstream << "|";
	for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
	{
		BB_FILE file = FILE_FromIndex(fileIndex);
		std::string fileString = FileToString(file);
		sstream << StringHelper::ToUpper(fileString);
	}
	sstream << " " << CastleFlagsToString(GetEngine().Position().Header.CastlingRights);
	if (GetEngine().Position().Header.EpFileIndex != FILEINDEX_NONE)
	{
		std::string epFileString = FileToString(FILE_FromIndex(GetEngine().Position().Header.EpFileIndex));
		sstream << " " << StringHelper::ToUpper(epFileString) << (GetEngine().Position().Header.MovingPlayer == PLAYER_WHITE ? "6" : "3");
	}
	else
		sstream << " -";
	sstream << std::endl;
	for (int i = 0; i < indentation; i++)
	{
		sstream << " ";
	}
	sstream << "Hash: " << StringHelper::ToHexString(GetEngine().Position().Hash) << std::endl;
	sstream << "FEN: " << PositionToFEN(GetEngine().Position()) << std::endl;
	return sstream.str();
}
