#include "Command_Divide.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>
#include <chrono>

Command_Divide::Command_Divide(Engine* pEngine) :
	Command(pEngine)
{

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
				MOVEGEN_GenerateMoves(&GetEngine().MoveGen(), &GetEngine().Position(), &moveList);
				std::uint64_t totalNodeCount = 0;
				for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
				{
					sstream << "  " << MoveToString(moveList.Move[moveIdx]) << ": ";
					MG_MOVEDATA moveData;
					MOVEGEN_MakeMove(&GetEngine().MoveGen(), moveList.Move[moveIdx], &moveData, &GetEngine().Position());
					std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
					std::uint64_t nodeCount = MOVEGEN_Perft(&GetEngine().Position(), &GetEngine().MoveGen(), depth - 1);
					std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
					std::chrono::high_resolution_clock::duration elapsed = end - start;
					std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
					MOVEGEN_UnmakeMove(&GetEngine().MoveGen(), moveList.Move[moveIdx], &moveData, &GetEngine().Position());
					sstream << nodeCount << " nodes in " << elapsedMs.count() << " milliseconds." << std::endl;
					totalNodeCount += nodeCount;
				}
				sstream << totalNodeCount << " nodes total." << std::endl;
				GetEngine().OutputStream() << sstream.str();
				return true;
			}
		}
	}
	return false;
}
