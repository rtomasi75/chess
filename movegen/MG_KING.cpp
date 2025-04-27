#include "MG_KING.h"
#include "MG_MOVEGEN.h"

BB_BITBOARD KING_MovesFromSquare(const BB_SQUARE& squareFrom)
{
	BB_BITBOARD targets =
		BITBOARD_DOWN(squareFrom) |
		BITBOARD_UP(squareFrom) |
		BITBOARD_LEFT(squareFrom) |
		BITBOARD_RIGHT(squareFrom) |
		BITBOARD_UPLEFT(squareFrom) |
		BITBOARD_DOWNLEFT(squareFrom) |
		BITBOARD_UPRIGHT(squareFrom) |
		BITBOARD_DOWNRIGHT(squareFrom);
	return targets;
}

size_t KING_CountMoves()
{
	size_t count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		BB_BITBOARD targets = KING_MovesFromSquare(squareFrom);
		count += BITBOARD_PopulationCount(targets);
	}
	return count * (1 + COUNT_PIECETYPES);
}

void KING_Initialize_Targets(MG_MOVEGEN* pMoveGen)
{
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = KING_MovesFromSquare(squareFrom);
		pMoveGen->JumpTargets[JUMPTARGETS_KING][squareIndexFrom] = targets;
	}
}

void KING_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = pMoveGen->JumpTargets[JUMPTARGETS_KING][squareIndexFrom];
		pMoveGen->JumpTable[JUMPTABLE_KING_QUIET].MovesBaseFrom[squareIndexFrom] = nextMove;
		pMoveGen->JumpTable[JUMPTABLE_KING_QUIET].TargetIndex = JUMPTARGETS_KING;
		BB_SQUARECOUNT count = BITBOARD_PopulationCount(targets);
		for (BB_SQUARECOUNT squareBit = 0; squareBit < count; squareBit++)
		{
			BB_BITBOARD squareBitIndex = UINT64_C(1) << squareBit;
			const BB_SQUARE squareTo = BITBOARD_BitDeposit(squareBitIndex, targets);
			const MG_MOVE move = nextMove++;
			const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
#endif
			pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
			pMoveGen->MoveTable[player][move].KillPiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].KillPlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].KillDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].MovePiece = PIECETYPE_KING;
			pMoveGen->MoveTable[player][move].MovePlayer = player;
			MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
		}
	}
}

void KING_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& piece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	const MG_PLAYER otherPlayer = PLAYER_OTHER(player);
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = pMoveGen->JumpTargets[JUMPTARGETS_KING][squareIndexFrom];
		pMoveGen->JumpTable[JUMPTABLE_KING_CAPTURE(piece)].MovesBaseFrom[squareIndexFrom] = nextMove;
		pMoveGen->JumpTable[JUMPTABLE_KING_CAPTURE(piece)].TargetIndex = JUMPTARGETS_KING;
		BB_SQUARECOUNT count = BITBOARD_PopulationCount(targets);
		for (BB_SQUARECOUNT squareBit = 0; squareBit < count; squareBit++)
		{
			BB_BITBOARD squareBitIndex = UINT64_C(1) << squareBit;
			const BB_SQUARE squareTo = BITBOARD_BitDeposit(squareBitIndex, targets);
			const MG_MOVE move = nextMove++;
			const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = squareTo;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
#endif
			pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
			pMoveGen->MoveTable[player][move].KillPiece = piece;
			pMoveGen->MoveTable[player][move].KillPlayer = otherPlayer;
			pMoveGen->MoveTable[player][move].KillDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MovePiece = PIECETYPE_KING;
			pMoveGen->MoveTable[player][move].MovePlayer = player;
			pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
			MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
		}
	}
}

void KING_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = true;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_JUMPTABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_JUMPTABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = JUMPTABLE_KING_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = JUMPTABLE_KING_CAPTURE(piece);
	}
}

