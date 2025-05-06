#include "MG_KNIGHT.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

BB_BITBOARD KNIGHT_MovesFromSquare(const BB_SQUARE& squareFrom)
{
	BB_BITBOARD targets =
		BITBOARD_LEFT(BITBOARD_DOWN(BITBOARD_DOWN(squareFrom))) |
		BITBOARD_RIGHT(BITBOARD_DOWN(BITBOARD_DOWN(squareFrom))) |
		BITBOARD_LEFT(BITBOARD_UP(BITBOARD_UP(squareFrom))) |
		BITBOARD_RIGHT(BITBOARD_UP(BITBOARD_UP(squareFrom))) |
		BITBOARD_LEFT(BITBOARD_LEFT(BITBOARD_DOWN(squareFrom))) |
		BITBOARD_RIGHT(BITBOARD_RIGHT(BITBOARD_DOWN(squareFrom))) |
		BITBOARD_LEFT(BITBOARD_LEFT(BITBOARD_UP(squareFrom))) |
		BITBOARD_RIGHT(BITBOARD_RIGHT(BITBOARD_UP(squareFrom)));
		return targets;
}

MG_MOVE KNIGHT_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	return JUMPTABLE_CountMoves(pMoveGen, JUMPTARGETS_KNIGHT);
}

void KNIGHT_Initialize_Targets(MG_MOVEGEN* pMoveGen)
{
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = KNIGHT_MovesFromSquare(squareFrom);
		pMoveGen->JumpTargets[JUMPTARGETS_KNIGHT][squareIndexFrom] = targets;
	}
}

void KNIGHT_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	JUMPTABLE_Initialize_QuietMoves(player, PIECETYPE_KNIGHT, pMoveGen, nextMove, JUMPTARGETS_KNIGHT, JUMPTABLE_KNIGHT_QUIET);
}

void KNIGHT_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	JUMPTABLE_Initialize_CaptureMoves(player, PIECETYPE_KNIGHT, capturedPiece, pMoveGen, nextMove, JUMPTARGETS_KNIGHT, JUMPTABLE_KNIGHT_CAPTURE(capturedPiece));
}

void KNIGHT_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = true;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_JUMPTABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_JUMPTABLE;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = JUMPTABLE_KNIGHT_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = JUMPTABLE_KNIGHT_CAPTURE(piece);
	}
}

