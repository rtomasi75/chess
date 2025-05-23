#include "Command_Perft.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include "../../libSearch.h"
#include <string>
#include <sstream>
#include <chrono>

Command_Perft::Command_Perft(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_Perft::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "perft")
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
				if (depth < 0)
				{
					GetEngine().ErrorMessage(depthString + " is not a depth (must be non-negative).");
					return true;
				}
				std::string squareString = StringHelper::ToLower(StringHelper::Trim(remainder));
				std::stringstream sstream;
				sstream << "Performing perft with depth " << depth << "." << std::endl;
				std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
				SE_POSITIONCOUNT nodeCount = 0;
				std::uint64_t leafCount = GetEngine().Perft(depth,nodeCount);
				std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				std::chrono::high_resolution_clock::duration elapsed = end - start;
				std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
				sstream << "  " << leafCount << " leafs in " << elapsedMs.count() << " milliseconds at depth " << depth << "." << std::endl;
				sstream << "  " << nodeCount << " nodes in " << elapsedMs.count() << " milliseconds at " << 1000.0 * static_cast<double>(nodeCount) / static_cast<double>(elapsedMs.count()) << " Node/s." << std::endl;
				GetEngine().OutputStream() << sstream.str();
				return true;
			}
		}
	}
	return false;
}
