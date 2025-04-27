#include "MG_MOVE.h"

MG_MOVE MOVE(const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& from, const BB_SQUARE& to, const MG_MOVETYPE& moveType)
{
	return UINT32_C(1) + ((((((std::uint32_t)player) * COUNT_PIECETYPES + ((std::uint32_t)piece)) * COUNT_MOVETYPES + ((std::uint32_t)moveType)) * COUNT_SQUARES + ((std::uint32_t)to)) * COUNT_SQUARES + ((std::uint32_t)from));
}

void MOVE_InitializeNullMove(MG_MOVEINFO* pMoveInfo, const MG_PLAYER& movingPlayer)
{
#ifndef MOVEGEN_COMPACT_MOVEINFO
	pMoveInfo[MOVE_NULLMOVE].KillMap = BITBOARD_EMPTY;
	pMoveInfo[MOVE_NULLMOVE].CreateMap = BITBOARD_EMPTY;
	pMoveInfo[MOVE_NULLMOVE].MoveMap = BITBOARD_EMPTY;
#endif
	pMoveInfo[MOVE_NULLMOVE].MoveDest = SQUAREINDEX_NONE;
	pMoveInfo[MOVE_NULLMOVE].MoveSource = SQUAREINDEX_NONE;
	pMoveInfo[MOVE_NULLMOVE].KillPiece = PIECETYPE_NONE;
	pMoveInfo[MOVE_NULLMOVE].KillPlayer = PLAYER_NONE;
	pMoveInfo[MOVE_NULLMOVE].KillDest = SQUAREINDEX_NONE;
	pMoveInfo[MOVE_NULLMOVE].CreatePiece = PIECETYPE_NONE;
	pMoveInfo[MOVE_NULLMOVE].CreatePlayer = PLAYER_NONE;
	pMoveInfo[MOVE_NULLMOVE].CreateDest = SQUAREINDEX_NONE;
	pMoveInfo[MOVE_NULLMOVE].MovePiece = PIECETYPE_NONE;
	pMoveInfo[MOVE_NULLMOVE].MovePlayer = movingPlayer;
	for (int strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		pMoveInfo[MOVE_NULLMOVE].MoveString[strPos] = 0;
	}
	pMoveInfo[MOVE_NULLMOVE].MoveString[0] = 'n';
	pMoveInfo[MOVE_NULLMOVE].MoveString[1] = 'u';
	pMoveInfo[MOVE_NULLMOVE].MoveString[2] = 'l';
	pMoveInfo[MOVE_NULLMOVE].MoveString[3] = 'l';
}
