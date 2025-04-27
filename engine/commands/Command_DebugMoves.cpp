#include "Command_DebugMoves.h"
#include "../StringHelper.h"
#include "../Engine.h"

Command_DebugMoves::Command_DebugMoves(Engine* pEngine) :
	Command(pEngine)
{

}

bool Command_DebugMoves::Try(const std::string& commandString)
{
	std::string input = commandString;
	if (StringHelper::ToLower(StringHelper::Trim(input)) == "debug-moves")
	{
		if (GetEngine().LegalMoves().CountMoves > 0)
		{
			GetEngine().OutputStream() << "Found " << ((int)GetEngine().LegalMoves().CountMoves) << " legal moves:" << std::endl;
			for (MG_MOVEINDEX moveIndex = 0; moveIndex < GetEngine().LegalMoves().CountMoves; moveIndex++)
			{
				MG_MOVE move = GetEngine().LegalMoves().Move[moveIndex];
				std::string movestring(GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].MoveString);
				GetEngine().OutputStream() << " - " << movestring << ":" << std::endl;

				GetEngine().OutputStream() << "    Move piece: " << PieceToString(GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].MovePiece, GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].MovePlayer) << std::endl;
				GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetMoveMap(&GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move]), 6);
				GetEngine().OutputStream() << "    Create piece: " << PieceToString(GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].CreatePiece, GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].CreatePlayer) << std::endl;
				GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetCreateMap(&GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move]), 6);
				GetEngine().OutputStream() << "    Kill piece: " << PieceToString(GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].KillPiece, GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move].KillPlayer) << std::endl;
				GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetKillMap(&GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move]), 6);
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
