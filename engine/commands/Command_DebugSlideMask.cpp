#include "Command_DebugSlideMask.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>

Command_DebugSlideMask::Command_DebugSlideMask(Engine* pEngine) :
	Command(pEngine)
{

}

std::string Command_DebugSlideMask::SlideMaskTableIndexToString(const int& index)
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

bool Command_DebugSlideMask::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "debug-slidemask")
		{
			std::string tableString;
			if (StringHelper::NextToken(remainder, tableString, remainder))
			{
				tableString = StringHelper::ToLower(StringHelper::Trim(tableString));
				int slideMaskIndex;
				try
				{
					slideMaskIndex = std::stoi(tableString);
				}
				catch (...)
				{
					GetEngine().ErrorMessage(tableString + " is not a valid slidemask-table index (failed to parse).");
					return true;
				}
				if (slideMaskIndex < 0)
				{
					GetEngine().ErrorMessage(tableString + " is not a valid slidemask-table index (must be non-negative).");
					return true;
				}
				if (slideMaskIndex >= COUNT_SLIDEMASKS)
				{
					std::stringstream ss;
					ss << (int)COUNT_SLIDEMASKS;
					GetEngine().ErrorMessage(tableString + " is not a valid slidemask-table index (must be below" + ss.str() + ").");
					return true;
				}
				std::string squareString = StringHelper::ToLower(StringHelper::Trim(remainder));
				BB_SQUARE square;
				if (ParseSquare(squareString, square))
				{
					std::string tableString = Command_DebugSlideMask::SlideMaskTableIndexToString(slideMaskIndex);
					GetEngine().OutputStream() << tableString << " for " << SquareToString(square) << ":" << std::endl;
					GetEngine().OutputStream() << "  Mask:" << std::endl;
					BB_SQUAREINDEX squareIndex = SQUARE_GetIndex(square);
					BB_BITBOARD targets = GetEngine().MoveGen().SlideMasks[slideMaskIndex].Mask[squareIndex];
					GetEngine().OutputStream() << BitboardToString(targets, 4);
				}
				else
				{
					GetEngine().ErrorMessage(squareString + " is not a valid square.");
				}
			}
			return true;
		}
	}
	return false;
}
