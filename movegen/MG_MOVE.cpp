#include "MG_MOVE.h"
#include "MG_MOVEINFO.h"
#include "MG_MOVEGEN.h"

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
}

void MOVE_InitializeNullMoveString(MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer)
{
	for (int strPos = 0; strPos < MOVESTRING_LENGTH; strPos++)
	{
		pMoveGen->MoveString[movingPlayer][strPos] = 0;
	}
	pMoveGen->MoveString[movingPlayer][0] = 'n';
	pMoveGen->MoveString[movingPlayer][1] = 'u';
	pMoveGen->MoveString[movingPlayer][2] = 'l';
	pMoveGen->MoveString[movingPlayer][3] = 'l';
}

