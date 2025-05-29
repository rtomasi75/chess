#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <sstream>
#include "libCommon.h"
#include "libMovegen.h"

class Engine;

class Command
{
private:
	Engine* _pEngine;
protected:
	Command(Engine* pEngine);
	Engine& GetEngine();
	const Engine& GetEngine() const;
	std::string RankToString(const BB_RANK& rank) const;
	std::string CastleFlagsToString(const MG_CASTLEFLAGS& castleFlags) const;
	std::string FileToString(const BB_FILE& file) const;
	std::string PlayerToString(const MG_PLAYER& player) const;
	std::string PieceToString(const MG_PIECETYPE& piece, const MG_PLAYER& player) const;
	std::string BitboardToString(const BB_BITBOARD& bitboard, int indentation = 0) const;
	bool ParseMove(const std::string& moveString, MG_MOVE& parsedMove) const;
	bool ParseSquare(const std::string& squareString, BB_SQUARE& parsedSquare) const;
	std::string MoveToString(const MG_MOVE& move) const;
	std::string MoveToHexString(const MG_MOVE& move) const;
	std::string PositionToString(const MG_POSITION& position, int indentation = 0) const;
	std::string PositionToFEN(const MG_POSITION& position) const;
	bool PositionFromFEN(MG_POSITION& position, const std::string& fen) const;
	std::string SquareToString(const BB_SQUARE& square) const;
	template<typename T>
	void PrintStructLayout(std::ostream& os, const std::string& name, int indentation = 0) const
	{
		std::stringstream sstream;
		for (int i = 0; i < indentation; i++)
			sstream << " ";
		sstream << name << ":" << std::endl;
		for (int i = 0; i < indentation; i++)
			sstream << " ";
		sstream << "  sizeof(" << name << ") = " << sizeof(T) << " bytes" << std::endl;
		for (int i = 0; i < indentation; i++)
			sstream << " ";
		sstream << "  alignof(" << name << ") = " << alignof(T) << std::endl;
		os << sstream.str();
	}
public:
	virtual ~Command() = default;
	virtual bool Try(const std::string& commandString) abstract;
};

#endif