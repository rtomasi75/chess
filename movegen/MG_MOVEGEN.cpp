#include "MG_MOVEGEN.h"
#include "MG_KING.h"
#include "MG_ROOK.h"
#include "MG_KNIGHT.h"
#include "MG_BISHOP.h"
#include "MG_QUEEN.h"
#include "libCommon.h"
#include <iostream>

MG_MOVE MOVEGEN_CountMoves(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& movingPlayer)
{
	MG_MOVE count = 1;
	const MG_MOVE countKingMoves = KING_CountMoves(pMoveGen);
	const MG_MOVE countRookMoves = ROOK_CountMoves(pMoveGen, movingPlayer);
	const MG_MOVE countKnightMoves = KNIGHT_CountMoves(pMoveGen);
	const MG_MOVE countBishopMoves = BISHOP_CountMoves(pMoveGen, movingPlayer);
	const MG_MOVE countQueenMoves = QUEEN_CountMoves(pMoveGen, movingPlayer);
	const MG_MOVE countPawnMoves = PAWN_CountMoves();
	count += countKingMoves;
	count += countRookMoves;
	count += countKnightMoves;
	count += countBishopMoves;
	count += countQueenMoves;
	count += countPawnMoves;
	return count;
}

void MOVEGEN_Initialize(MG_MOVEGEN* pMoveGen)
{
	pMoveGen->CountSlideEntries = SLIDEMASKS_CountEntries();
	pMoveGen->SlideEntries = new MG_SLIDEENTRY[pMoveGen->CountSlideEntries];
	MG_SLIDEENTRYINDEX nextEntry = 0;
	SLIDEMASKS_Initialize(pMoveGen, nextEntry);
	ROOK_Initialize_LookUps(pMoveGen);
	BISHOP_Initialize_LookUps(pMoveGen);
	QUEEN_Initialize_LookUps(pMoveGen);
	KING_Initialize_Targets(pMoveGen);
	KNIGHT_Initialize_Targets(pMoveGen);
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		// Allocate tables
		KING_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_KING]);
		KNIGHT_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_KNIGHT]);
		ROOK_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_ROOK]);
		BISHOP_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_BISHOP]);
		QUEEN_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_QUEEN]);
		PAWN_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_PAWN]);
		pMoveGen->CountMoves[movingPlayer] = MOVEGEN_CountMoves(pMoveGen, movingPlayer);
		pMoveGen->MoveTable[movingPlayer] = new MG_MOVEINFO[pMoveGen->CountMoves[movingPlayer]];
	}
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		// nullmove
		MG_MOVE nextMove = MOVE_NULLMOVE;
		MOVE_InitializeNullMove(pMoveGen->MoveTable[movingPlayer], movingPlayer);
		nextMove++;
		// King
		KING_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			KING_Initialize_CaptureMoves(movingPlayer, capturedPiece, pMoveGen, nextMove);
		}
		KING_Initialize_CastleMoves(movingPlayer, pMoveGen, nextMove);
		// Knight
		KNIGHT_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			KNIGHT_Initialize_CaptureMoves(movingPlayer, capturedPiece, pMoveGen, nextMove);
		}
		// Rook
		ROOK_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		ROOK_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
		// Bishop
		BISHOP_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		BISHOP_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
		// Queen
		QUEEN_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		QUEEN_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
		// Pawn
		PAWN_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		PAWN_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
		ASSERT(nextMove == pMoveGen->CountMoves[movingPlayer]);
	}
}

void MOVEGEN_Deinitialize(MG_MOVEGEN* pMoveGen)
{
	delete[] pMoveGen->SlideEntries;
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		delete[] pMoveGen->MoveTable[movingPlayer];
	}
}

void MOVEGEN_GenerateMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, MG_MOVELIST* pMoveList)
{
	const MG_PLAYER movingPlayer = pPosition->MovingPlayer;
	MOVELIST_Initialize(pMoveList);
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		switch (pMoveGen->PieceInfo[movingPlayer][piece].MoveMechanic[MOVETYPE_QUIET])
		{
		case MOVEMECHANIC_CASTLE:
		default:
			ASSERT(false);
			break;
		case MOVEMECHANIC_JUMPTABLE:
			JUMPTABLE_GenerateQuietMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_SLIDETABLE:
			SLIDEMASKS_GenerateQuietMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_PAWN:
			PAWN_GenerateQuietMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_NONE:
			break;
		}
		switch (pMoveGen->PieceInfo[movingPlayer][piece].MoveMechanic[MOVETYPE_CAPTURE])
		{
		case MOVEMECHANIC_CASTLE:
		default:
			ASSERT(false);
			break;
		case MOVEMECHANIC_JUMPTABLE:
			JUMPTABLE_GenerateCaptureMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_SLIDETABLE:
			SLIDEMASKS_GenerateCaptureMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_PAWN:
			PAWN_GenerateCaptureMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_NONE:
			break;
		}
		switch (pMoveGen->PieceInfo[movingPlayer][piece].MoveMechanic[MOVETYPE_SPECIAL])
		{
		default:
		case MOVEMECHANIC_JUMPTABLE:
		case MOVEMECHANIC_PAWN:
		case MOVEMECHANIC_SLIDETABLE:
			ASSERT(false);
			break;
		case MOVEMECHANIC_CASTLE:
			KING_GenerateCastleMoves(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_NONE:
			break;
		}
	}
}

MG_OPTIONINDEX MOVEGEN_OptionIndex(const BB_SQUARE& square, const BB_BITBOARD& targets)
{
	const std::uint64_t optionIndex = CM_BitExtract(square, targets);
	return CM_BitScanForward(optionIndex);
}

bool MOVEGEN_ParseMoveString(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVELIST* pMoveList, const char* pString, const int& len, int& strPos, MG_MOVE& outParsed)
{
	int found = -1;
	for (MG_MOVEINDEX moveIndex = 0; moveIndex < pMoveList->CountMoves; moveIndex++)
	{
		int localPos = 0;
		MG_MOVE move = pMoveList->Move[moveIndex];
		found = moveIndex;
		while (localPos < MOVESTRING_LENGTH)
		{
			if (pMoveGen->MoveTable[player][move].MoveString[localPos] == 0)
			{
				break;
			}
			if ((localPos + strPos) >= len)
			{
				found = -1;
			}
			if (pMoveGen->MoveTable[player][move].MoveString[localPos] != pString[localPos + strPos])
			{
				found = -1;
				break;
			}
			localPos++;
		}
		if (found >= 0)
		{
			outParsed = pMoveList->Move[moveIndex];
			return true;
		}
	}
	return false;
}

void MOVEGEN_MakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_MOVEDATA* pOutMoveData, MG_POSITION* pPosition)
{
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->MovingPlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
		interestMap |= moveMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
		interestMap |= killMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
	const MG_CASTLEFLAGS oldFlags = pPosition->CastlingRights;
	const MG_CASTLEFLAGS newFlags = oldFlags & moveInfo.CastleRightsMask;
	const BB_FILEINDEX oldEpFile = pPosition->EpFileIndex;
	pOutMoveData->OldHash = pPosition->Hash;
	pOutMoveData->OldCastlingRights = oldFlags;
	pOutMoveData->OldEnPassantFile = oldEpFile;
	pPosition->EpFileIndex = moveInfo.EnPassantFileIndex;
	pPosition->Hash ^= HASH_MOVINGPLAYER_BLACK;
	pPosition->Hash ^= moveInfo.HashDelta;
	pPosition->Hash ^= HASH_CastleRights(oldFlags) ^ HASH_CastleRights(newFlags);
	pPosition->Hash ^= HASH_EnPassantFile(oldEpFile) ^ HASH_EnPassantFile(moveInfo.EnPassantFileIndex);
	pPosition->CastlingRights = newFlags;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (interestMap & pPosition->InterestPlayerPiece[player][piece])
			{
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
		pPosition->AttacksPlayer[player] = attacks;
	}
}

void MOVEGEN_UnmakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_MOVEDATA* pMoveData, MG_POSITION* pPosition)
{
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->PassivePlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
		interestMap |= moveMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
		interestMap |= killMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
	pPosition->Hash = pMoveData->OldHash;
	pPosition->CastlingRights = pMoveData->OldCastlingRights;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (interestMap & pPosition->InterestPlayerPiece[player][piece])
			{
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
		pPosition->AttacksPlayer[player] = attacks;
	}
}

void MOVEGEN_MakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_POSITION* pPosition)
{
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->MovingPlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
		interestMap |= moveMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
		interestMap |= killMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (interestMap & pPosition->InterestPlayerPiece[player][piece])
			{
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
		pPosition->AttacksPlayer[player] = attacks;
	}
}

void MOVEGEN_UnmakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_POSITION* pPosition)
{
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->PassivePlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
		interestMap |= moveMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
		interestMap |= killMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (interestMap & pPosition->InterestPlayerPiece[player][piece])
			{
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
		pPosition->AttacksPlayer[player] = attacks;
	}
}

BB_BITBOARD MOVEGEN_GetPieceAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest)
{
	switch (pMoveGen->PieceInfo[player][piece].MoveMechanic[MOVETYPE_CAPTURE])
	{
	default:
		ASSERT(false);
		return BITBOARD_EMPTY;
	case MOVEMECHANIC_JUMPTABLE:
		return JUMPTABLE_GetPieceAttacks(pMoveGen, pPosition, piece, player, outInterest);
	case MOVEMECHANIC_SLIDETABLE:
		return SLIDEMASKS_GetPieceAttacks(pMoveGen, pPosition, piece, player, outInterest);
	case MOVEMECHANIC_PAWN:
		return PAWN_GetPawnAttacks(pMoveGen, pPosition, piece, player, outInterest);
	case MOVEMECHANIC_NONE:
		outInterest = BITBOARD_EMPTY;
		return BITBOARD_EMPTY;
	}
}

void MOVEGEN_RecomputeAttacks(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PLAYER& player)
{
	BB_BITBOARD totalAttacks = BITBOARD_EMPTY;
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		const BB_BITBOARD pieceAttacks = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
		pPosition->AttacksPlayerPiece[player][piece] = pieceAttacks;
		totalAttacks |= pieceAttacks;
	}
	pPosition->AttacksPlayer[player] = totalAttacks;
}

void MOVEGEN_FinalizeMove(const MG_MOVEGEN* pMoveGen, MG_MOVELIST* pMoveList, MG_POSITION* pPosition, const MG_MOVE& move)
{
	ASSERT(pMoveList->CountMoves < MAX_MOVES);
#ifdef MOVGEN_LEGALMOVES
	MOVEGEN_MakeTentativeMove(pMoveGen, move, pPosition);
	const bool bLegal = POSITION_IsLegal(pPosition);
	MOVEGEN_UnmakeTentativeMove(pMoveGen, move, pPosition);
	if (bLegal)
		pMoveList->Move[pMoveList->CountMoves++] = move;
#else
	pMoveList->Move[pMoveList->CountMoves++] = move;
#endif
}

std::uint64_t MOVEGEN_Perft(MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen, int depth)
{
	if (depth == 0)
		return 1;
	uint64_t nodes = 0;
	MG_MOVELIST moveList;
	MOVELIST_Initialize(&moveList);
	MOVEGEN_GenerateMoves(pMoveGen, pPosition, &moveList);
	for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
	{
		const MG_MOVE move = moveList.Move[moveIdx];
		MG_MOVEDATA moveData;
		MOVEGEN_MakeMove(pMoveGen, move, &moveData, pPosition);
#ifdef MOVGEN_LEGALMOVES
		nodes += MOVEGEN_Perft(pPosition, pMoveGen, depth - 1);
#else
		if (POSITION_IsLegal(pPosition))
		{
			nodes += MOVEGEN_Perft(pPosition, pMoveGen, depth - 1);
		}
#endif
		MOVEGEN_UnmakeMove(pMoveGen, move, &moveData, pPosition);
	}
	return nodes;
}