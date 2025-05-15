#include "Command_DebugMoves.h"
#include "../StringHelper.h"
#include "../Engine.h"
#include <sstream>

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
			std::stringstream sstream;
			sstream << "Found " << ((int)GetEngine().LegalMoves().CountMoves) << " legal moves:" << std::endl;
			for (MG_MOVEINDEX moveIndex = 0; moveIndex < GetEngine().LegalMoves().CountMoves; moveIndex++)
			{
				MG_MOVE move = GetEngine().LegalMoves().Move[moveIndex];
				sstream << " - " << MoveToString(move) << "(" << MoveToHexString(move) << ")" << ":" << std::endl;
				const MG_MOVEINFO& moveInfo = GetEngine().MoveGen().MoveTable[GetEngine().MovingPlayer()][move];
				if (moveInfo.MovePiece != PIECETYPE_NONE)
				{
					sstream << "    Move piece: " << PieceToString(moveInfo.MovePiece, moveInfo.MovePlayer) << std::endl;
					sstream << BitboardToString(MOVEINFO_GetMoveMap(&moveInfo), 6);
				}
				if (moveInfo.CreatePiece != PIECETYPE_NONE)
				{
					sstream << "    Create piece: " << PieceToString(moveInfo.CreatePiece, moveInfo.CreatePlayer) << std::endl;
					sstream << BitboardToString(MOVEINFO_GetCreateMap(&moveInfo), 6);
				}
				if (moveInfo.KillPiece != PIECETYPE_NONE)
				{
					sstream << "    Kill piece: " << PieceToString(moveInfo.KillPiece, moveInfo.KillPlayer) << std::endl;
					sstream << BitboardToString(MOVEINFO_GetKillMap(&moveInfo), 6);
				}
				if (moveInfo.PromoPiece != PIECETYPE_NONE)
				{
					sstream << "    Promo piece: " << PieceToString(moveInfo.PromoPiece, moveInfo.PromoPiece) << std::endl;
					sstream << BitboardToString(MOVEINFO_GetPromoMap(&moveInfo), 6);
				}
			}
			GetEngine().OutputStream() << sstream.str();
		}
		else
		{
			GetEngine().OutputStream() << "Found no legal moves." << std::endl;
		}
		return true;
	}
	return false;
}
