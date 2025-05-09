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
				const MG_MOVEINFO& moveInfo = GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move];
				if (moveInfo.MovePiece != PIECETYPE_NONE)
				{
					GetEngine().OutputStream() << "    Move piece: " << PieceToString(moveInfo.MovePiece, moveInfo.MovePlayer) << std::endl;
					GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetMoveMap(&moveInfo), 6);
				}
				if (moveInfo.CreatePiece != PIECETYPE_NONE)
				{
					GetEngine().OutputStream() << "    Create piece: " << PieceToString(moveInfo.CreatePiece, moveInfo.CreatePlayer) << std::endl;
					GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetCreateMap(&moveInfo), 6);
				}
				if (moveInfo.KillPiece != PIECETYPE_NONE)
				{
					GetEngine().OutputStream() << "    Kill piece: " << PieceToString(moveInfo.KillPiece, moveInfo.KillPlayer) << std::endl;
					GetEngine().OutputStream() << BitboardToString(MOVEINFO_GetKillMap(&moveInfo), 6);
				}
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
