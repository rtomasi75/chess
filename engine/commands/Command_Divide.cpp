#include "Command_Divide.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <algorithm>

Command_Divide::Command_Divide(Engine* pEngine) :
	Command(pEngine)
{

}

void Command_Divide::SortMoves(MG_MOVELIST& moveList)
{
	std::vector<MG_MOVE> moves;
	for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
	{
		moves.emplace_back(moveList.Move[moveIdx]);
	}
	std::sort(moves.begin(), moves.end(), [this](MG_MOVE arg1, MG_MOVE arg2)
		{
			return MoveToString(arg1) < MoveToString(arg2);
		});
	for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
	{
		moveList.Move[moveIdx] = moves[moveIdx];
	}
}

bool Command_Divide::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "divide")
		{
			if (StringHelper::Trim(remainder) != "")
			{
				std::string depthString;
				depthString = StringHelper::ToLower(StringHelper::Trim(remainder));
				int depth;
				try
				{
					depth = std::stoi(depthString);
				}
				catch (...)
				{
					GetEngine().ErrorMessage(depthString + " is not a depth (failed to parse).");
					return true;
				}
				if (depth < 1)
				{
					GetEngine().ErrorMessage(depthString + " is not a depth (must be positive).");
					return true;
				}
				std::string squareString = StringHelper::ToLower(StringHelper::Trim(remainder));
				std::stringstream sstream;
				sstream << "Performing divide with depth " << depth << "." << std::endl;
				MG_MOVELIST moveList;
				MG_POSITION position = GetEngine().Position();
				MOVEGEN_GenerateMoves(&GetEngine().MoveGen(), &position, &moveList);
				SortMoves(moveList);
				std::uint64_t totalNodeCount = 0;
				for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
				{
					MG_MOVEDATA moveData;
					MOVEGEN_MakeMove(&GetEngine().MoveGen(), moveList.Move[moveIdx], &moveData, &position);
#ifndef MOVEGEN_LEGAL
					if (POSITION_IsLegal(&position))
					{
						sstream << "  " << MoveToString(moveList.Move[moveIdx]) << ": ";
						std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
						std::uint64_t nodeCount = 0;
						std::uint64_t leafCount = SEARCH_PerftRoot(&position, &GetEngine().MoveGen(), depth - 1, nodeCount);
						std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
						std::chrono::high_resolution_clock::duration elapsed = end - start;
						std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
						totalNodeCount += leafCount;
						sstream << leafCount << " leafs in " << elapsedMs.count() << " milliseconds with " << 1000.0 * static_cast<double>(nodeCount) / static_cast<double>(elapsedMs.count()) << " Node/s." << std::endl;
					}
#else
					sstream << "  " << MoveToString(moveList.Move[moveIdx]) << ": ";
					std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
					std::uint64_t nodeCount = 0;
					std::uint64_t leafCount = SEARCH_PerftRoot(&position, &GetEngine().MoveGen(), depth - 1, nodeCount);
					std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
					std::chrono::high_resolution_clock::duration elapsed = end - start;
					std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
					totalNodeCount += leafCount;
					sstream << leafCount << " leafs in " << elapsedMs.count() << " milliseconds with " << 1000.0 * static_cast<double>(nodeCount) / static_cast<double>(elapsedMs.count()) << " Node/s." << std::endl;
#endif
					MOVEGEN_UnmakeMove(&GetEngine().MoveGen(), moveList.Move[moveIdx], &moveData, &position);
				}
				sstream << totalNodeCount << " nodes total and " << moveList.CountMoves << " moves at root." << std::endl;
				GetEngine().OutputStream() << sstream.str();
				return true;
			}
		}
	}
	return false;
}
