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
	const MG_MOVE countCastleMoves = 2;
	return JUMPTABLE_CountMoves(pMoveGen, JUMPTARGETS_KING) + countCastleMoves;
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
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_CASTLE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = JUMPTABLE_KING_QUIET;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = JUMPTABLE_KING_CAPTURE(piece);
	}
}

void KING_Initialize_CastleMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	pMoveGen->CastleBase[player] = nextMove;
	const MG_MOVE moveKingSide = nextMove++;
	const MG_MOVE moveQueenSide = nextMove++;
	const BB_SQUARE squareToKingSide = (player == PLAYER_WHITE) ? SQUARE_G1 : SQUARE_G8;
	const BB_SQUARE squareToQueenSide = (player == PLAYER_WHITE) ? SQUARE_C1 : SQUARE_C8;
	const BB_SQUARE squareFrom = (player == PLAYER_WHITE) ? SQUARE_E1 : SQUARE_E8;
	const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
	const BB_SQUAREINDEX squareIndexToKingSide = SQUARE_GetIndex(squareToKingSide);
	const BB_SQUAREINDEX squareIndexToQueenSide = SQUARE_GetIndex(squareToQueenSide);
	ASSERT(moveKingSide < pMoveGen->CountMoves[player]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
	pMoveGen->MoveTable[player][moveKingSide].KillMap = (player == PLAYER_WHITE) ? SQUARE_H1 : SQUARE_H8;
	pMoveGen->MoveTable[player][moveKingSide].CreateMap = (player == PLAYER_WHITE) ? SQUARE_F1 : SQUARE_F8;
	pMoveGen->MoveTable[player][moveKingSide].PromoMap = BITBOARD_EMPTY;
	pMoveGen->MoveTable[player][moveKingSide].MoveMap = squareFrom ^ squareToKingSide;
#endif
	pMoveGen->MoveTable[player][moveKingSide].MoveDest = squareIndexToKingSide;
	pMoveGen->MoveTable[player][moveKingSide].MoveSource = squareIndexFrom;
	pMoveGen->MoveTable[player][moveKingSide].KillPiece = PIECETYPE_ROOK;
	pMoveGen->MoveTable[player][moveKingSide].KillPlayer = player;
	pMoveGen->MoveTable[player][moveKingSide].KillDest = SQUARE_GetIndex((player == PLAYER_WHITE) ? SQUARE_H1 : SQUARE_H8);
	pMoveGen->MoveTable[player][moveKingSide].PromoPiece = PIECETYPE_NONE;
	pMoveGen->MoveTable[player][moveKingSide].PromoPlayer = PLAYER_NONE;
	pMoveGen->MoveTable[player][moveKingSide].PromoSource = SQUAREINDEX_NONE;
	pMoveGen->MoveTable[player][moveKingSide].CreatePiece = PIECETYPE_ROOK;
	pMoveGen->MoveTable[player][moveKingSide].CreatePlayer = player;
	pMoveGen->MoveTable[player][moveKingSide].CreateDest = SQUARE_GetIndex((player == PLAYER_WHITE) ? SQUARE_F1 : SQUARE_F8);
	pMoveGen->MoveTable[player][moveKingSide].MovePiece = PIECETYPE_KING;
	pMoveGen->MoveTable[player][moveKingSide].ResetHalfMoveClock = true;
	pMoveGen->MoveTable[player][moveKingSide].MovePlayer = player;
	pMoveGen->MoveTable[player][moveKingSide].EnPassantFileIndex = FILEINDEX_NONE;
	pMoveGen->MoveTable[player][moveKingSide].HashDelta = HASH_PlayerPieceSquare(player, PIECETYPE_KING, squareFrom) ^ HASH_PlayerPieceSquare(player, PIECETYPE_KING, squareToKingSide) ^ HASH_PlayerPieceSquare(player, PIECETYPE_ROOK, (player == PLAYER_WHITE) ? SQUARE_H1 : SQUARE_H8) ^ HASH_PlayerPieceSquare(player, PIECETYPE_ROOK, (player == PLAYER_WHITE) ? SQUARE_F1 : SQUARE_F8);
	pMoveGen->MoveTable[player][moveKingSide].CastleRightsMask = ~((player == PLAYER_WHITE) ? (CASTLEFLAGS_WHITE_KINGSIDE | CASTLEFLAGS_WHITE_QUEENSIDE) : (CASTLEFLAGS_BLACK_KINGSIDE | CASTLEFLAGS_BLACK_QUEENSIDE));
	MOVEINFO_InitializeMoveString(pMoveGen, player, moveKingSide, squareFrom, squareToKingSide);
	ASSERT(moveQueenSide < pMoveGen->CountMoves[player]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
	pMoveGen->MoveTable[player][moveQueenSide].KillMap = (player == PLAYER_WHITE) ? SQUARE_A1 : SQUARE_A8;
	pMoveGen->MoveTable[player][moveQueenSide].CreateMap = (player == PLAYER_WHITE) ? SQUARE_D1 : SQUARE_D8;
	pMoveGen->MoveTable[player][moveQueenSide].PromoMap = BITBOARD_EMPTY;
	pMoveGen->MoveTable[player][moveQueenSide].MoveMap = squareFrom ^ squareToQueenSide;
#endif
	pMoveGen->MoveTable[player][moveQueenSide].MoveDest = squareIndexToQueenSide;
	pMoveGen->MoveTable[player][moveQueenSide].MoveSource = squareIndexFrom;
	pMoveGen->MoveTable[player][moveQueenSide].KillPiece = PIECETYPE_ROOK;
	pMoveGen->MoveTable[player][moveQueenSide].KillPlayer = player;
	pMoveGen->MoveTable[player][moveQueenSide].KillDest = SQUARE_GetIndex((player == PLAYER_WHITE) ? SQUARE_A1 : SQUARE_A8);
	pMoveGen->MoveTable[player][moveQueenSide].PromoPiece = PIECETYPE_NONE;
	pMoveGen->MoveTable[player][moveQueenSide].PromoPlayer = PLAYER_NONE;
	pMoveGen->MoveTable[player][moveQueenSide].PromoSource = SQUAREINDEX_NONE;
	pMoveGen->MoveTable[player][moveQueenSide].CreatePiece = PIECETYPE_ROOK;
	pMoveGen->MoveTable[player][moveQueenSide].CreatePlayer = player;
	pMoveGen->MoveTable[player][moveQueenSide].CreateDest = SQUARE_GetIndex((player == PLAYER_WHITE) ? SQUARE_D1 : SQUARE_D8);
	pMoveGen->MoveTable[player][moveQueenSide].MovePiece = PIECETYPE_KING;
	pMoveGen->MoveTable[player][moveKingSide].ResetHalfMoveClock = true;
	pMoveGen->MoveTable[player][moveQueenSide].MovePlayer = player;
	pMoveGen->MoveTable[player][moveQueenSide].EnPassantFileIndex = FILEINDEX_NONE;
	pMoveGen->MoveTable[player][moveQueenSide].HashDelta = HASH_PlayerPieceSquare(player, PIECETYPE_KING, squareFrom) ^ HASH_PlayerPieceSquare(player, PIECETYPE_KING, squareToQueenSide) ^ HASH_PlayerPieceSquare(player, PIECETYPE_ROOK, (player == PLAYER_WHITE) ? SQUARE_A1 : SQUARE_A8) ^ HASH_PlayerPieceSquare(player, PIECETYPE_ROOK, (player == PLAYER_WHITE) ? SQUARE_D1 : SQUARE_D8);
	pMoveGen->MoveTable[player][moveQueenSide].CastleRightsMask = ~((player == PLAYER_WHITE) ? (CASTLEFLAGS_WHITE_KINGSIDE | CASTLEFLAGS_WHITE_QUEENSIDE) : (CASTLEFLAGS_BLACK_KINGSIDE | CASTLEFLAGS_BLACK_QUEENSIDE));
	MOVEINFO_InitializeMoveString(pMoveGen, player, moveQueenSide, squareFrom, squareToQueenSide);
}

#define CASTLEWALK_KINGSIDE_WHITE (SQUARE_F1|SQUARE_G1)
#define CASTLEWALK_KINGSIDE_BLACK (SQUARE_F8|SQUARE_G8)
#define CASTLEWALK_QUEENSIDE_WHITE (SQUARE_D1|SQUARE_C1|SQUARE_B1)
#define CASTLEWALK_QUEENSIDE_BLACK (SQUARE_D8|SQUARE_C8|SQUARE_B8)

#define CASTLEINTEREST_KINGSIDE_WHITE (SQUARE_E1|SQUARE_F1|SQUARE_G1)
#define CASTLEINTEREST_KINGSIDE_BLACK (SQUARE_E8|SQUARE_F8|SQUARE_G8)
#define CASTLEINTEREST_QUEENSIDE_WHITE (SQUARE_E1|SQUARE_D1|SQUARE_C1)
#define CASTLEINTEREST_QUEENSIDE_BLACK (SQUARE_E8|SQUARE_D8|SQUARE_C8)

void KING_GenerateCastleMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	ASSERT(piece == PIECETYPE_KING);
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	const MG_PLAYER passivePlayer = pPosition->Header.PassivePlayer;
	const bool bIsWhite = (movingPlayer == PLAYER_WHITE);
	const bool bIsBlack = !bIsWhite;
	const MG_CASTLEFLAGS kingSideFlag = (bIsWhite * CASTLEFLAGS_WHITE_KINGSIDE) | (bIsBlack * CASTLEFLAGS_BLACK_KINGSIDE);
	const BB_BITBOARD kingSideWalk = (bIsWhite * CASTLEWALK_KINGSIDE_WHITE) | (bIsBlack * CASTLEWALK_KINGSIDE_BLACK);
	const BB_BITBOARD kingSideInterest = (bIsWhite * CASTLEINTEREST_KINGSIDE_WHITE) | (bIsBlack * CASTLEINTEREST_KINGSIDE_BLACK);
	const BB_BITBOARD occupancyTotal = pPosition->OccupancyTotal;
	const BB_BITBOARD attacks = pPosition->AttacksPlayer[passivePlayer];
	const MG_CASTLEFLAGS castlingRights = pPosition->Header.CastlingRights;
	const bool bCastleKingSide = (castlingRights & kingSideFlag) && !(occupancyTotal & kingSideWalk) && !(attacks & kingSideInterest);
	if (bCastleKingSide)
	{
		MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, pMoveGen->CastleBase[movingPlayer]);
	}
	const MG_CASTLEFLAGS queenSideFlag = (bIsWhite * CASTLEFLAGS_WHITE_QUEENSIDE) | (bIsBlack * CASTLEFLAGS_BLACK_QUEENSIDE);
	const BB_BITBOARD queenSideWalk = (bIsWhite * CASTLEWALK_QUEENSIDE_WHITE) | (bIsBlack * CASTLEWALK_QUEENSIDE_BLACK);
	const BB_BITBOARD queenSideInterest = (bIsWhite * CASTLEINTEREST_QUEENSIDE_WHITE) | (bIsBlack * CASTLEINTEREST_QUEENSIDE_BLACK);
	const bool bCastleQueenSide = (castlingRights & queenSideFlag) && !(occupancyTotal & queenSideWalk) && !(attacks & queenSideInterest);
	if (bCastleQueenSide)
	{
		MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, pMoveGen->CastleBase[movingPlayer] + 1);
	}
}

