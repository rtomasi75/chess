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
	return countPushes;
}

MG_MOVE PAWN_CountCaptureMoves()
{
	const MG_MOVE countCaptures = (COUNT_RANKS - 3) * COUNT_FILES * 2 * COUNT_PIECETYPES;
	return countCaptures;
}

MG_MOVE PAWN_CountMoves()
{
	return PAWN_CountQuietMoves() + PAWN_CountCaptureMoves();
}

void PAWN_Initialize_QuietMoves(const MG_PLAYER& player, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove)
{
	pMoveGen->PawnTable[player].QuietBase = nextMove;
	// normal pushes
	if (player == PLAYER_WHITE)
	{
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
				ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareTo;
#endif
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveDest = squareIndexTo;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MoveSource = squareIndexFrom;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillPlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].KillDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePiece = PIECETYPE_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreatePlayer = PLAYER_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].CreateDest = SQUAREINDEX_NONE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_WHITE][move].MovePlayer = PLAYER_WHITE;
				pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_WHITE][move].MoveString, squareFrom, squareTo);
			}
		}
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
				const BB_SQUARE squareTo = BITBOARD_DOWN(squareFrom);
				const BB_SQUAREINDEX squareIndexFrom = SQUARE_GetIndex(squareFrom);
				const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
				const MG_MOVE move = nextMove++;
				ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
				pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = BITBOARD_EMPTY;
				pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
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
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePiece = PIECETYPE_PAWN;
				pMoveGen->MoveTable[PLAYER_BLACK][move].MovePlayer = PLAYER_BLACK;
				pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareTo);
				pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareTo);
				MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_BLACK][move].MoveString, squareFrom, squareTo);
			}
		}
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
						ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
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
						pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToLeft);
						pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_WHITE][move].MoveString, squareFrom, squareToLeft);
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
						ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_WHITE][move].KillMap = squareTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].MoveMap = squareFrom ^ squareTo;
						pMoveGen->MoveTable[PLAYER_WHITE][move].CreateMap = BITBOARD_EMPTY;
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
						pMoveGen->MoveTable[PLAYER_WHITE][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, PIECETYPE_PAWN, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, capturedPiece, squareToRight);
						pMoveGen->MoveTable[PLAYER_WHITE][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_WHITE, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_WHITE][move].MoveString, squareFrom, squareToRight);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
			}
		}
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
						ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
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
						pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareToLeft) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToLeft);
						pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToLeft, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_BLACK][move].MoveString, squareFrom, squareToLeft);
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
						ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
						pMoveGen->MoveTable[PLAYER_BLACK][move].KillMap = squareTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].MoveMap = squareFrom ^ squareTo;
						pMoveGen->MoveTable[PLAYER_BLACK][move].CreateMap = BITBOARD_EMPTY;
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
						pMoveGen->MoveTable[PLAYER_BLACK][move].HashDelta = HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom) ^ HASH_PlayerPieceSquare(PLAYER_BLACK, PIECETYPE_PAWN, squareToRight) ^ HASH_PlayerPieceSquare(PLAYER_WHITE, capturedPiece, squareToRight);
						pMoveGen->MoveTable[PLAYER_BLACK][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(PLAYER_BLACK, PIECETYPE_PAWN, squareFrom, squareToRight, capturedPiece);
						MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[PLAYER_BLACK][move].MoveString, squareFrom, squareToRight);
					}
				}
				else
					nextMove += COUNT_PIECETYPES;
			}
		}
	}
}

void PAWN_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	ASSERT(piece == PIECETYPE_PAWN);
	if (pPosition->MovingPlayer == PLAYER_WHITE)
	{
		BB_BITBOARD movers = pPosition->OccupancyPlayerPiece[PLAYER_WHITE][PIECETYPE_PAWN] & ~RANK_7;
		BB_SQUAREINDEX fromSquareIndex;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const MG_MOVE offset = (fromRankIndex - 1) * COUNT_FILES + fromFileIndex;
			const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].QuietBase + offset;
			ASSERT(move < pMoveGen->CountMoves);
			pMoveList->Move[pMoveList->CountMoves++] = move;
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
			const MG_MOVE offset = (fromRankIndex - 2) * COUNT_FILES + fromFileIndex;
			const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].QuietBase + offset;
			ASSERT(move < pMoveGen->CountMoves);
			pMoveList->Move[pMoveList->CountMoves++] = move;
		}
	}
}

void PAWN_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	ASSERT(piece == PIECETYPE_PAWN);
	if (pPosition->MovingPlayer == PLAYER_WHITE)
	{
		BB_BITBOARD movers = pPosition->OccupancyPlayerPiece[PLAYER_WHITE][PIECETYPE_PAWN] & ~RANK_7;
		BB_SQUAREINDEX fromSquareIndex;
		while (SQUARE_Next(movers, fromSquareIndex))
		{
			const BB_FILEINDEX fromFileIndex = SQUARE_GetFileIndex(fromSquareIndex);
			const BB_RANKINDEX fromRankIndex = SQUARE_GetRankIndex(fromSquareIndex);
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE offset = ((fromRankIndex - 1) * COUNT_FILES + fromFileIndex) * COUNT_PIECETYPES * 2;
			for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD targetsLeft = BITBOARD_UP(BITBOARD_LEFT(squareFrom)) & pPosition->OccupancyPlayerPiece[PLAYER_BLACK][capturedPiece];
				const BB_BITBOARD targetsRight = BITBOARD_UP(BITBOARD_RIGHT(squareFrom)) & pPosition->OccupancyPlayerPiece[PLAYER_BLACK][capturedPiece];
				if (targetsLeft)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].CaptureBase + offset + capturedPiece;
					ASSERT(move < pMoveGen->CountMoves);
					pMoveList->Move[pMoveList->CountMoves++] = move;
				}
				if (targetsRight)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_WHITE].CaptureBase + offset + capturedPiece + COUNT_PIECETYPES;
					ASSERT(move < pMoveGen->CountMoves);
					pMoveList->Move[pMoveList->CountMoves++] = move;
				}
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
			const BB_SQUARE squareFrom = SQUARE_FromIndex(fromSquareIndex);
			const MG_MOVE offset = ((fromRankIndex - 2) * COUNT_FILES + fromFileIndex) * COUNT_PIECETYPES * 2;
			for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
			{
				const BB_BITBOARD targetsLeft = BITBOARD_DOWN(BITBOARD_LEFT(squareFrom)) & pPosition->OccupancyPlayerPiece[PLAYER_WHITE][capturedPiece];
				const BB_BITBOARD targetsRight = BITBOARD_DOWN(BITBOARD_RIGHT(squareFrom)) & pPosition->OccupancyPlayerPiece[PLAYER_WHITE][capturedPiece];
				if (targetsLeft)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_BLACK].CaptureBase + offset + capturedPiece;
					ASSERT(move < pMoveGen->CountMoves);
					pMoveList->Move[pMoveList->CountMoves++] = move;
				}
				if (targetsRight)
				{
					const MG_MOVE move = pMoveGen->PawnTable[PLAYER_BLACK].CaptureBase + offset + capturedPiece + COUNT_PIECETYPES;
					ASSERT(move < pMoveGen->CountMoves);
					pMoveList->Move[pMoveList->CountMoves++] = move;
				}
			}
		}
	}
}
