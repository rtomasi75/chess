#include "Command_DebugSlideTargets.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>

Command_DebugSlideTargets::Command_DebugSlideTargets(Engine* pEngine) :
	Command(pEngine)
{

}

std::string Command_DebugSlideTargets::SlideMaskTableIndexToString(const int& index)
{
	switch (index)
	{
	case SLIDEMASKS_DIAGONAL:
		return "SLIDEMASKS_DIAGONAL";
	case SLIDEMASKS_HORIZONTAL:
		return "SLIDEMASKS_HORIZONTAL(PIECETYPE_KING)";
	default:
		throw std::runtime_error("Invalid index.");
	}
}

bool Command_DebugSlideTargets::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "debug-slidetargets")
		{
			std::string squareString = StringHelper::ToLower(StringHelper::Trim(remainder));
			BB_SQUARE square;
			if (ParseSquare(squareString, square))
			{
				std::stringstream sstream;
				sstream << "Slide-targets for " << SquareToString(square) << ":" << std::endl;
				BB_SQUAREINDEX squareIndex = SQUARE_GetIndex(square);
				sstream << "  Occupancy:" << std::endl;
				sstream << BitboardToString(GetEngine().Position().OccupancyTotal, 4);
				sstream << "  Diagonal targets:" << std::endl;
				sstream << BitboardToString(SLIDEMASKS_GenerateTargetsDiagonal(square, GetEngine().Position().OccupancyTotal), 4);
				sstream << "  Horizontal targets:" << std::endl;
				sstream << BitboardToString(SLIDEMASKS_GenerateTargetsHorizontal(square, GetEngine().Position().OccupancyTotal), 4);
				GetEngine().OutputStream() << sstream.str();
			}
			else
			{
				GetEngine().ErrorMessage(squareString + " is not a valid square.");
			}
			return true;
		}
	}
	return false;
}
