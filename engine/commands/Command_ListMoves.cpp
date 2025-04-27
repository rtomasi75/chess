#include "Command_ListMoves.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_ListMoves::Command_ListMoves(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_ListMoves::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "listmoves")
	{
		if (GetEngine().LegalMoves().CountMoves > 0)
		{
			GetEngine().OutputStream() << "Found " << ((int)GetEngine().LegalMoves().CountMoves) << " legal moves:" << std::endl;
			for (MG_MOVEINDEX moveIndex = 0; moveIndex < GetEngine().LegalMoves().CountMoves; moveIndex++)
			{
				MG_MOVE move = GetEngine().LegalMoves().Move[moveIndex];
				GetEngine().OutputStream() << "  " << MoveToString(move) << std::endl;
			}

		}
		else
		{
			GetEngine().OutputStream() << "Found no legal moves." << std::endl;
		}
		return true;
	}
	return false;
}
