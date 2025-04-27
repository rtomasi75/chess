#include "Command_DebugJumps.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <string>
#include <sstream>

Command_DebugJumps::Command_DebugJumps(Engine* pEngine) :
	Command(pEngine)
{

}

std::string Command_DebugJumps::JumpTableIndexToString(const int& index)
{
	switch (index)
	{
	case JUMPTABLE_KING_QUIET:
		return "JUMPTABLE_KING_QUIET";
	case JUMPTABLE_KING_CAPTURE(PIECETYPE_KING):
		return "JUMPTABLE_KING_CAPTURE(PIECETYPE_KING)";
	case JUMPTABLE_KING_CAPTURE(PIECETYPE_ROOK):
		return "JUMPTABLE_KING_CAPTURE(PIECETYPE_ROOK)";
	default:
		throw std::runtime_error("Invalid index.");
	}
}

bool Command_DebugJumps::Try(const std::string& commandString)
{
	std::string input = commandString;
	std::string token;
	std::string remainder;
	if (StringHelper::NextToken(input, token, remainder))
	{
		if (StringHelper::ToLower(StringHelper::Trim(token)) == "debug-jumps")
		{
			std::string tableString;
			if (StringHelper::NextToken(remainder, tableString, remainder))
			{
				tableString = StringHelper::ToLower(StringHelper::Trim(tableString));
				int jumpTableIndex;
				try
				{
					jumpTableIndex = std::stoi(tableString);
				}
				catch (...)
				{
					GetEngine().ErrorMessage(tableString + " is not a valid jump-table index (failed to parse).");
					return true;
				}
				if (jumpTableIndex < 0)
				{
					GetEngine().ErrorMessage(tableString + " is not a valid jump-table index (must be non-negative).");
					return true;
				}
				if (jumpTableIndex >= COUNT_JUMPTABLES)
				{
					std::stringstream ss;
					ss << (int)COUNT_JUMPTABLES;
					GetEngine().ErrorMessage(tableString + " is not a valid jump-table index (must be below" + ss.str() + ").");
					return true;
				}
				std::string squareString = StringHelper::ToLower(StringHelper::Trim(remainder));
				BB_SQUARE square;
				if (ParseSquare(squareString, square))
				{
					std::string tableString = Command_DebugJumps::JumpTableIndexToString(jumpTableIndex);
					GetEngine().OutputStream() << tableString << " for " << SquareToString(square) << ":" << std::endl;
					MG_JUMPTARGETSINDEX targetIndex = GetEngine().MoveGen().JumpTable[jumpTableIndex].TargetIndex;
					GetEngine().OutputStream() << "  Target index: " << targetIndex << std::endl;
					GetEngine().OutputStream() << "  Mask:" << std::endl;
					BB_SQUAREINDEX squareIndex = SQUARE_GetIndex(square);
					BB_BITBOARD targets = GetEngine().MoveGen().JumpTargets[targetIndex][squareIndex];
					GetEngine().OutputStream() << BitboardToString(targets, 4);
					MG_MOVE baseMove = GetEngine().MoveGen().JumpTable[jumpTableIndex].MovesBaseFrom[squareIndex];
					GetEngine().OutputStream() << "  BaseMove: " << MoveToHexString(baseMove) << std::endl;
					GetEngine().OutputStream() << "  Moves:" << std::endl;
					BB_BITBOARD remainingTargets = targets;
					BB_SQUAREINDEX targetSquareIndex;
					while (SQUARE_Next(remainingTargets, targetSquareIndex))
					{
						BB_SQUARE targetSquare = SQUARE_FromIndex(targetSquareIndex);
						MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(targetSquare, targets);
						MG_MOVE move = baseMove + optionIndex;
						GetEngine().OutputStream() << "    " << MoveToHexString(move) << ": " << MoveToString(move) << std::endl;
					}
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
