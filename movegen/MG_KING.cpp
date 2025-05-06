#include "MG_KING.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

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

MG_MOVE KING_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	return JUMPTABLE_CountMoves(pMoveGen, JUMPTARGETS_KING);
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
	JUMPTABLE_Initialize_QuietMoves(player, PIECETYPE_KING, pMoveGen, nextMove, JUMPTARGETS_KING, JUMPTABLE_KING_QUIET);
}

void KING_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	JUMPTABLE_Initialize_CaptureMoves(player, PIECETYPE_KING, capturedPiece, pMoveGen, nextMove, JUMPTARGETS_KING, JUMPTABLE_KING_CAPTURE(capturedPiece));
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

