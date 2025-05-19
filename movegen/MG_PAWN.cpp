#include "MG_PAWN.h"
#include "MG_MOVEGEN.h"
#include "libCommon.h"

void PAWN_Initialize_PieceInfo(MG_PIECEINFO* pPieceInfo)
{
	pPieceInfo->IsRoyal = false;
	pPieceInfo->MoveMechanic[MOVETYPE_QUIET] = MOVEMECHANIC_PAWN;
	pPieceInfo->MoveMechanic[MOVETYPE_CAPTURE] = MOVEMECHANIC_PAWN;
	pPieceInfo->MoveMechanic[MOVETYPE_SPECIAL] = MOVEMECHANIC_NONE;
	pPieceInfo->TableIndex[TABLEINDEX_QUIET] = 0;
	pPieceInfo->TableIndex[TABLEINDEX_SPECIAL] = 0;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		pPieceInfo->TableIndex[TABLEINDEX_CAPTURE(piece)] = 0;
	}
}


MG_MOVE PAWN_CountQuietMoves()
{
	const MG_MOVE countPushes = (COUNT_RANKS - 3) * COUNT_FILES;
	const MG_MOVE countPromotions = COUNT_FILES * MOVEGEN_COUNT_PROMOPIECES;
	const MG_MOVE countDoublePushes = COUNT_FILES;
	return countPushes + countPromotions + countDoublePushes;
}

MG_MOVE PAWN_CountCaptureMoves()
{
	const MG_MOVE countCaptures = (COUNT_RANKS - 3) * COUNT_FILES * 2 * COUNT_PIECETYPES;
	const MG_MOVE countPromoCaptures = COUNT_FILES * 2 * COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES;
	const MG_MOVE countEnPassantCaptures = COUNT_FILES * 2;
	return countCaptures + countPromoCaptures + countEnPassantCaptures;
}

MG_MOVE PAWN_CountMoves()
{
	return PAWN_CountQuietMoves() + PAWN_CountCaptureMoves();
}

void PAWN_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	// normal pushes
	if (player == PLAYER_WHITE)
	{
		pMoveGen->PawnTable[PLAYER_WHITE].QuietBase = nextMove;
		for (BB_RANKINDEX rankIndex = 1; rankIndex < COUNT_RANKS - 2; rankIndex++)
		{
			const BB_RANK rank = RANK_FromIndex(rankIndex);
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_UP(squareFrom);
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareTo;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
				pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFrom, squareTo);
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].QuietBase) == ((COUNT_RANKS - 3) * COUNT_FILES));
		pMoveGen->PawnTable[PLAYER_WHITE].PromotionBase = nextMove;
		{
			const BB_RANK rank = RANK_7;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_UP(squareFrom);
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
				{
					const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = squareFrom;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = squareTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = squareIndexFrom;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = promoPiece;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, promoPiece, squareTo);
					pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareTo);
					MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_WHITE, move, squareFrom, squareTo, promoPiece);
				}
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].PromotionBase) == (COUNT_FILES * MOVEGEN_COUNT_PROMOPIECES));
		pMoveGen->PawnTable[PLAYER_WHITE].DoublePushBase = nextMove;
		{
			const BB_RANK rank = RANK_2;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_UP(BITBOARD_UP(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareTo;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = fileIndex;
				pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFrom, squareTo);
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].DoublePushBase) == COUNT_FILES);
	}
	else
	{
		pMoveGen->PawnTable[PLAYER_BLACK].QuietBase = nextMove;
		for (BB_RANKINDEX rankIndex = 2; rankIndex < COUNT_RANKS - 1; rankIndex++)
		{
			const BB_RANK rank = RANK_FromIndex(rankIndex);
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_DOWN(squareFrom);
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareTo;
#endif
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
				pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFrom, squareTo);
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].QuietBase) == ((COUNT_RANKS - 3) * COUNT_FILES));
		pMoveGen->PawnTable[PLAYER_BLACK].PromotionBase = nextMove;
		{
			const BB_RANK rank = RANK_2;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_DOWN(squareFrom);
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
				{
					const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = squareFrom;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = squareTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = squareIndexFrom;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = promoPiece;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, promoPiece, squareTo);
					pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareTo);
					MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_BLACK, move, squareFrom, squareTo, promoPiece);
				}
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].PromotionBase) == (COUNT_FILES * MOVEGEN_COUNT_PROMOPIECES));
		pMoveGen->PawnTable[PLAYER_BLACK].DoublePushBase = nextMove;
		{
			const BB_RANK rank = RANK_7;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareTo = BITBOARD_DOWN(BITBOARD_DOWN(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareTo;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
				pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = fileIndex;
				pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFrom, squareTo);
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].DoublePushBase) == COUNT_FILES);
	}
}

void PAWN_Initialize_CaptureMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	pMoveGen->PawnTable[player].CaptureBase = nextMove;
	if (player == PLAYER_WHITE)
	{
		for (BB_RANKINDEX rankIndex = 1; rankIndex < COUNT_RANKS - 2; rankIndex++)
		{
			const BB_RANK rank = RANK_FromIndex(rankIndex);
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareToLeft = BITBOARD_LEFT(BITBOARD_UP(squareFrom));
				const BB_SQUARE squareToRight = BITBOARD_RIGHT(BITBOARD_UP(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				if (squareToLeft)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToLeft);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						const MG_MOVE move = nextMove++;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareToLeft;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareToLeft;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = BITBOARD_EMPTY;
#endif
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFrom;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = capturedPiece;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToLeft);
						pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFrom, squareToLeft);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
				if (squareToRight)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToRight);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						const MG_MOVE move = nextMove++;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareToRight;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareToRight;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = BITBOARD_EMPTY;
#endif
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFrom;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = capturedPiece;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToRight);
						pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFrom, squareToRight);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].CaptureBase) == ((COUNT_RANKS - 3) * COUNT_FILES * 2 * COUNT_PIECETYPES));
		pMoveGen->PawnTable[PLAYER_WHITE].PromoCaptureBase = nextMove;
		{
			const BB_RANK rank = RANK_7;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareToLeft = BITBOARD_LEFT(BITBOARD_UP(squareFrom));
				const BB_SQUARE squareToRight = BITBOARD_RIGHT(BITBOARD_UP(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				if (squareToLeft)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToLeft);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
						{
							const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
							const MG_MOVE move = nextMove++;
							ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareToLeft;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = BITBOARD_EMPTY;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = squareFrom;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = squareToLeft;
#endif
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = capturedPiece;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = promoPiece;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_PAWN;
							pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = squareIndexFrom;
							pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, promoPiece, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToLeft);
							pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
							MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_WHITE, move, squareFrom, squareToLeft, promoPiece);
						}
					}
				}
				else
					nextMove += COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES;
				if (squareToRight)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToRight);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
						{
							const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
							const MG_MOVE move = nextMove++;
							ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareToRight;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = BITBOARD_EMPTY;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = squareFrom;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = squareToRight;
#endif
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = capturedPiece;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = promoPiece;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_PAWN;
							pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = squareIndexFrom;
							pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, promoPiece, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToRight);
							pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
							MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_WHITE, move, squareFrom, squareToRight, promoPiece);
						}
					}
				}
				else
					nextMove += COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].PromoCaptureBase) == (COUNT_FILES * 2 * COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES));
		pMoveGen->PawnTable[PLAYER_WHITE].EnPassantBase = nextMove;
		{
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareTo = SQUARE_FromRankFile(RANK_6, file);
				const BB_SQUARE squareCapture = SQUARE_FromRankFile(RANK_5, file);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const BB_SQUAREINDEX squareIndexCapture = SQUARE_GetIndex(squareCapture);
				const BB_SQUARE squareFromLeft = BITBOARD_LEFT(SQUARE_FromRankFile(RANK_5, file));
				const BB_SQUARE squareFromRight = BITBOARD_RIGHT(SQUARE_FromRankFile(RANK_5, file));
				if (squareFromLeft)
				{
					const BB_SQUAREINDEX squareIndexFromLeft = SQUARE_GetIndex(squareFromLeft);
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareCapture;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFromLeft ^ squareTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFromLeft;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexCapture;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFromLeft) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareTo) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareCapture);
					pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFromLeft, squareTo, PIECETYPE_PAWN);
					MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFromLeft, squareTo);
				}
				else
					nextMove++;
				if (squareFromRight)
				{
					const BB_SQUAREINDEX squareIndexFromRight = SQUARE_GetIndex(squareFromRight);
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareCapture;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFromRight ^ squareTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFromRight;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = squareIndexCapture;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].PromoSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFromRight) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareTo) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareCapture);
					pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFromRight, squareTo, PIECETYPE_PAWN);
					MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_WHITE, move, squareFromRight, squareTo);
				}
				else
					nextMove++;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_WHITE].EnPassantBase) == (COUNT_FILES * 2));
	}
	else
	{
		for (BB_RANKINDEX rankIndex = 2; rankIndex < COUNT_RANKS - 1; rankIndex++)
		{
			const BB_RANK rank = RANK_FromIndex(rankIndex);
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareToLeft = BITBOARD_LEFT(BITBOARD_DOWN(squareFrom));
				const BB_SQUARE squareToRight = BITBOARD_RIGHT(BITBOARD_DOWN(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				if (squareToLeft)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToLeft);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						const MG_MOVE move = nextMove++;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareToLeft;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareToLeft;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFrom;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = capturedPiece;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToLeft);
						pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFrom, squareToLeft);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
				if (squareToRight)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToRight);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						const MG_MOVE move = nextMove++;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareToRight;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareToRight;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFrom;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = capturedPiece;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
						pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToRight);
						pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFrom, squareToRight);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].CaptureBase) == ((COUNT_RANKS - 3) * COUNT_FILES * 2 * COUNT_PIECETYPES));
		pMoveGen->PawnTable[PLAYER_BLACK].PromoCaptureBase = nextMove;
		{
			const BB_RANK rank = RANK_2;
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareFrom = SQUARE_FromRankFile(rank, file);
				const BB_SQUARE squareToLeft = BITBOARD_LEFT(BITBOARD_DOWN(squareFrom));
				const BB_SQUARE squareToRight = BITBOARD_RIGHT(BITBOARD_DOWN(squareFrom));
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				if (squareToLeft)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToLeft);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
						{
							const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
							const MG_MOVE move = nextMove++;
							ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareToLeft;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = BITBOARD_EMPTY;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = squareFrom;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = squareToLeft;
#endif
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = capturedPiece;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = promoPiece;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_PAWN;
							pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = squareIndexFrom;
							pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, promoPiece, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToLeft);
							pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
							MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_BLACK, move, squareFrom, squareToLeft, promoPiece);
						}
					}
				}
				else
					nextMove += COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES;
				if (squareToRight)
				{
					const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareToRight);
					for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
					{
						for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
						{
							const MG_PIECETYPE promoPiece = PAWN_PromoPieces[promoPieceIndex];
							const MG_MOVE move = nextMove++;
							ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareToRight;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = BITBOARD_EMPTY;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = squareFrom;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = squareToRight;
#endif
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = SQUAREINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = capturedPiece;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = promoPiece;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = squareIndexTo;
							pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_PAWN;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_BLACK;
							pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = squareIndexFrom;
							pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
							pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, promoPiece, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToRight);
							pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
							MOVEINFO_InitializeMoveStringPromotion(pMoveGen, PLAYER_BLACK, move, squareFrom, squareToRight, promoPiece);
						}
					}
				}
				else
					nextMove += COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].PromoCaptureBase) == (COUNT_FILES * 2 * COUNT_PIECETYPES * MOVEGEN_COUNT_PROMOPIECES));
		pMoveGen->PawnTable[PLAYER_BLACK].EnPassantBase = nextMove;
		{
			for (BB_FILEINDEX fileIndex = 0; fileIndex < COUNT_FILES; fileIndex++)
			{
				const BB_FILE file = FILE_FromIndex(fileIndex);
				const BB_SQUARE squareTo = SQUARE_FromRankFile(RANK_3, file);
				const BB_SQUARE squareCapture = SQUARE_FromRankFile(RANK_4, file);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const BB_SQUAREINDEX squareIndexCapture = SQUARE_GetIndex(squareCapture);
				const BB_SQUARE squareFromLeft = BITBOARD_LEFT(SQUARE_FromRankFile(RANK_4, file));
				const BB_SQUARE squareFromRight = BITBOARD_RIGHT(SQUARE_FromRankFile(RANK_4, file));
				if (squareFromLeft)
				{
					const BB_SQUAREINDEX squareIndexFromLeft = SQUARE_GetIndex(squareFromLeft);
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareCapture;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFromLeft ^ squareTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFromLeft;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexCapture;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFromLeft) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareTo) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareCapture);
					pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFromLeft, squareTo, PIECETYPE_PAWN);
					MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFromLeft, squareTo);
				}
				else
					nextMove++;
				if (squareFromRight)
				{
					const BB_SQUAREINDEX squareIndexFromRight = SQUARE_GetIndex(squareFromRight);
					const MG_MOVE move = nextMove++;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareCapture;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFromRight ^ squareTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoMap = BITBOARD_EMPTY;
#endif
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveDest = squareIndexTo;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MoveSource = squareIndexFromRight;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillPlayer = PLAYER_WHITE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].KillDest = squareIndexCapture;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
					pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreatePlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].ResetHalfMoveClock = true;
					pMoveGen->MoveTable[PLAYER_BLACK][move].CreateDest = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPiece = PIECETYPE_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoPlayer = PLAYER_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].PromoSource = SQUAREINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].EnPassantFileIndex = FILEINDEX_NONE;
					pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFromRight) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareTo) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareCapture);
					pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFromRight, squareTo, PIECETYPE_PAWN);
					MOVEINFO_InitializeMoveString(pMoveGen, PLAYER_BLACK, move, squareFromRight, squareTo);
				}
				else
					nextMove++;
			}
		}
		ASSERT((nextMove - pMoveGen->PawnTable[PLAYER_BLACK].EnPassantBase) == (COUNT_FILES * 2));
	}
}

void PAWN_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	ASSERT(piece == PIECETYPE_PAWN);
	const BB_BITBOARD totalOccupancy = pPosition->OccupancyTotal;
	if (pPosition->Header.MovingPlayer == PLAYER_WHITE)
	{
		BB_BITBOARD movers = pPosition->OccupancyPlayerPiece[PLAYER_WHITE][PIECETYPE_PAWN] & ~RANK_7;
		BB_SQUAREINDEX fromSquareIndex;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareTo = BITBOARD_UP(SQUARE_FromRankFileIndices(fromRankIndex, fromFileIndex));
			if (!(totalOccupancy & squareTo))
			{
				const MG_MOVE offset = (fromRankIndex - 1) * COUNT_FILES + fromFileIndex;
				const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].QuietBase + offset;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
		movers = pPosition->OccupancyPlayerPiece[PLAYER_WHITE][PIECETYPE_PAWN] & RANK_7;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareTo = BITBOARD_UP(SQUARE_FromRankFileIndices(fromRankIndex, fromFileIndex));
			if (!(totalOccupancy & squareTo))
			{
				const MG_MOVE offset = fromFileIndex * MOVEGEN_COUNT_PROMOPIECES;
				for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].PromotionBase + offset + promoPieceIndex;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
			}
		}
		movers = pPosition->OccupancyPlayerPiece[PLAYER_WHITE][PIECETYPE_PAWN] & RANK_2;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_SQUARE squareOver = BITBOARD_UP(SQUARE_FromIndex(fromSquareIndex));
			const BB_SQUARE squareTo = BITBOARD_UP(squareOver);
			if (!(totalOccupancy & (squareTo | squareOver)))
			{
				const MG_MOVE offset = fromFileIndex;
				const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].DoublePushBase + offset;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
	else
	{
		BB_BITBOARD movers = pPosition->OccupancyPlayerPiece[PLAYER_BLACK][PIECETYPE_PAWN] & ~RANK_2;
		BB_SQUAREINDEX fromSquareIndex;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareTo = BITBOARD_DOWN(SQUARE_FromRankFileIndices(fromRankIndex, fromFileIndex));
			if (!(totalOccupancy & squareTo))
			{
				const MG_MOVE offset = (fromRankIndex - 2) * COUNT_FILES + fromFileIndex;
				const MG_MOVE move = pMoveGen->PawnTable[PLAYER_BLACK].QuietBase + offset;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
		movers = pPosition->OccupancyPlayerPiece[PLAYER_BLACK][PIECETYPE_PAWN] & RANK_2;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareTo = BITBOARD_DOWN(SQUARE_FromRankFileIndices(fromRankIndex, fromFileIndex));
			if (!(totalOccupancy & squareTo))
			{
				const MG_MOVE offset = fromFileIndex * MOVEGEN_COUNT_PROMOPIECES;
				for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_BLACK].PromotionBase + offset + promoPieceIndex;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
			}
		}
		movers = pPosition->OccupancyPlayerPiece[PLAYER_BLACK][PIECETYPE_PAWN] & RANK_7;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_SQUARE squareOver = BITBOARD_DOWN(SQUARE_FromIndex(fromSquareIndex));
			const BB_SQUARE squareTo = BITBOARD_DOWN(squareOver);
			if (!(totalOccupancy & (squareTo | squareOver)))
			{
				const MG_MOVE offset = fromFileIndex;
				const MG_MOVE move = pMoveGen->PawnTable[PLAYER_BLACK].DoublePushBase + offset;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
}

void PAWN_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	ASSERT(piece == PIECETYPE_PAWN);
	const BB_BITBOARD* pPlayerOccupancyBlack = pPosition->OccupancyPlayerPiece[PLAYER_BLACK];
	const BB_BITBOARD* pPlayerOccupancyWhite = pPosition->OccupancyPlayerPiece[PLAYER_WHITE];
	if (pPosition->Header.MovingPlayer == PLAYER_WHITE)
	{
		BB_BITBOARD movers = pPlayerOccupancyWhite[PIECETYPE_PAWN] & ~RANK_7;
		BB_SQUAREINDEX fromSquareIndex;
		const MG_MOVE captureBase = pMoveGen->PawnTable[PLAYER_WHITE].CaptureBase;
		const MG_MOVE promoCaptureBase = pMoveGen->PawnTable[PLAYER_WHITE].PromoCaptureBase;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE fileRankOffset = (fromRankIndex - 1) * COUNT_FILES + fromFileIndex;
			const MG_MOVE offset = fileRankOffset * COUNT_PIECETYPES * 2;
			for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD enemyOccupancy = pPlayerOccupancyBlack[capturedPiece];
				const BB_BITBOARD targetsLeft = BITBOARD_UP(BITBOARD_LEFT(squareFrom)) & enemyOccupancy;
				const BB_BITBOARD targetsRight = BITBOARD_UP(BITBOARD_RIGHT(squareFrom)) & enemyOccupancy;
				if (targetsLeft)
				{
					const MG_MOVE move = captureBase + offset + capturedPiece;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
				if (targetsRight)
				{
					const MG_MOVE move = captureBase + offset + capturedPiece + COUNT_PIECETYPES;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
			}
		}
		movers = pPlayerOccupancyWhite[PIECETYPE_PAWN] & RANK_7;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE fileRankOffset = fromFileIndex;
			const MG_MOVE offset = fileRankOffset * COUNT_PIECETYPES * 2 * MOVEGEN_COUNT_PROMOPIECES;
			for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD enemyOccupancy = pPlayerOccupancyBlack[capturedPiece];
				const BB_BITBOARD targetsLeft = BITBOARD_UP(BITBOARD_LEFT(squareFrom)) & enemyOccupancy;
				const BB_BITBOARD targetsRight = BITBOARD_UP(BITBOARD_RIGHT(squareFrom)) & enemyOccupancy;
				if (targetsLeft)
				{
					for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
					{
						const MG_MOVE move = promoCaptureBase + offset + capturedPiece * MOVEGEN_COUNT_PROMOPIECES + promoPieceIndex;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
						MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
					}
				}
				if (targetsRight)
				{
					for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
					{
						const MG_MOVE move = promoCaptureBase + offset + (COUNT_PIECETYPES + capturedPiece) * MOVEGEN_COUNT_PROMOPIECES + promoPieceIndex;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
						MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
					}
				}
			}
		}
		if (pPosition->Header.EpFileIndex != FILEINDEX_NONE)
		{
			const BB_FILE captureFile = FILE_FromIndex(pPosition->Header.EpFileIndex);
			const BB_SQUARE captureSquare = SQUARE_FromRankFile(RANK_6, captureFile);
			const BB_SQUARE targetSquare = BITBOARD_DOWN(captureSquare);
			const BB_SQUARE fromSquareLeft = BITBOARD_LEFT(targetSquare);
			const BB_SQUARE fromSquareRight = BITBOARD_RIGHT(targetSquare);
			const MG_MOVE enPassantBase = pMoveGen->PawnTable[PLAYER_WHITE].EnPassantBase + 2 * pPosition->Header.EpFileIndex;
			if (fromSquareLeft & pPlayerOccupancyWhite[PIECETYPE_PAWN])
			{
				const MG_MOVE move = enPassantBase;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
			if (fromSquareRight & pPlayerOccupancyWhite[PIECETYPE_PAWN])
			{
				const MG_MOVE move = enPassantBase + 1;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_WHITE]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
	else
	{
		BB_BITBOARD movers = pPlayerOccupancyBlack[PIECETYPE_PAWN] & ~RANK_2;
		BB_SQUAREINDEX fromSquareIndex;
		const MG_MOVE captureBase = pMoveGen->PawnTable[PLAYER_BLACK].CaptureBase;
		const MG_MOVE promoCaptureBase = pMoveGen->PawnTable[PLAYER_BLACK].PromoCaptureBase;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE fileRankOffset = (fromRankIndex - 2) * COUNT_FILES + fromFileIndex;
			const MG_MOVE offset = fileRankOffset * COUNT_PIECETYPES * 2;
			for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD enemyOccupancy = pPlayerOccupancyWhite[capturedPiece];
				const BB_BITBOARD targetsLeft = BITBOARD_DOWN(BITBOARD_LEFT(squareFrom)) & enemyOccupancy;
				const BB_BITBOARD targetsRight = BITBOARD_DOWN(BITBOARD_RIGHT(squareFrom)) & enemyOccupancy;
				if (targetsLeft)
				{
					const MG_MOVE move = captureBase + offset + capturedPiece;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
				if (targetsRight)
				{
					const MG_MOVE move = captureBase + offset + capturedPiece + COUNT_PIECETYPES;
					ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
					MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
				}
			}
		}
		movers = pPlayerOccupancyBlack[PIECETYPE_PAWN] & RANK_2;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE fileRankOffset = fromFileIndex;
			const MG_MOVE offset = fileRankOffset * COUNT_PIECETYPES * 2 * MOVEGEN_COUNT_PROMOPIECES;
			for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD enemyOccupancy = pPlayerOccupancyWhite[capturedPiece];
				const BB_BITBOARD targetsLeft = BITBOARD_DOWN(BITBOARD_LEFT(squareFrom)) & enemyOccupancy;
				const BB_BITBOARD targetsRight = BITBOARD_DOWN(BITBOARD_RIGHT(squareFrom)) & enemyOccupancy;
				if (targetsLeft)
				{
					for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
					{
						const MG_MOVE move = promoCaptureBase + offset + capturedPiece * MOVEGEN_COUNT_PROMOPIECES + promoPieceIndex;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
						MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
					}
				}
				if (targetsRight)
				{
					for (int promoPieceIndex = 0; promoPieceIndex < MOVEGEN_COUNT_PROMOPIECES; promoPieceIndex++)
					{
						const MG_MOVE move = promoCaptureBase + offset + (COUNT_PIECETYPES + capturedPiece) * MOVEGEN_COUNT_PROMOPIECES + promoPieceIndex;
						ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
						MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
					}
				}
			}
		}
		if (pPosition->Header.EpFileIndex != FILEINDEX_NONE)
		{
			const BB_FILE captureFile = FILE_FromIndex(pPosition->Header.EpFileIndex);
			const BB_SQUARE captureSquare = SQUARE_FromRankFile(RANK_3, captureFile);
			const BB_SQUARE targetSquare = BITBOARD_UP(captureSquare);
			const BB_SQUARE fromSquareLeft = BITBOARD_LEFT(targetSquare);
			const BB_SQUARE fromSquareRight = BITBOARD_RIGHT(targetSquare);
			const MG_MOVE enPassantBase = pMoveGen->PawnTable[PLAYER_BLACK].EnPassantBase + 2 * pPosition->Header.EpFileIndex;
			if (fromSquareLeft & pPlayerOccupancyBlack[PIECETYPE_PAWN])
			{
				const MG_MOVE move = enPassantBase;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
			if (fromSquareRight & pPlayerOccupancyBlack[PIECETYPE_PAWN])
			{
				const MG_MOVE move = enPassantBase + 1;
				ASSERT(move < pMoveGen->CountMoves[PLAYER_BLACK]);
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
}

BB_BITBOARD PAWN_GetPawnAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest)
{
	ASSERT(piece == PIECETYPE_PAWN);
	const BB_BITBOARD pawns = pPosition->OccupancyPlayerPiece[player][PIECETYPE_PAWN];
	const BB_BITBOARD pushed = (player == PLAYER_WHITE) ? BITBOARD_UP(pawns) : BITBOARD_DOWN(pawns);
	outInterest = pawns;
	return BITBOARD_LEFT(pushed) | BITBOARD_RIGHT(pushed);
}
