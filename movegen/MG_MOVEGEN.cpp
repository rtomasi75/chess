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
		pMoveGen->MoveString[movingPlayer] = new char[pMoveGen->CountMoves[movingPlayer] * MOVESTRING_LENGTH];
	}
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		// nullmove
		MG_MOVE nextMove = MOVE_NULLMOVE;
		MOVE_InitializeNullMove(pMoveGen->MoveTable[movingPlayer], movingPlayer);
		MOVE_InitializeNullMoveString(pMoveGen, movingPlayer);
		nextMove++;
		// Pawn
		PAWN_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		PAWN_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
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
		ASSERT(nextMove == pMoveGen->CountMoves[movingPlayer]);
	}
}

void MOVEGEN_Deinitialize(MG_MOVEGEN* pMoveGen)
{
	delete[] pMoveGen->SlideEntries;
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		delete[] pMoveGen->MoveTable[movingPlayer];
		delete[] pMoveGen->MoveString[movingPlayer];
	}
}

typedef void(*MG_GenerateMovesFn)(const MG_MOVEGEN*, MG_POSITION*, const MG_PIECETYPE&, MG_MOVELIST*);

static inline void MOVEGEN_GenerateNoMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
}

static constexpr MG_GenerateMovesFn g_GenerateQuietMovesFn[COUNT_MOVEMECHANICS] =
{
	MOVEGEN_GenerateNoMoves,
	JUMPTABLE_GenerateQuietMoves,
	SLIDEMASKS_GenerateQuietMoves,
	PAWN_GenerateQuietMoves,
	MOVEGEN_GenerateNoMoves
};

static constexpr MG_GenerateMovesFn g_GenerateCaptureMovesFn[COUNT_MOVEMECHANICS] =
{
	MOVEGEN_GenerateNoMoves,
	JUMPTABLE_GenerateCaptureMoves,
	SLIDEMASKS_GenerateCaptureMoves,
	PAWN_GenerateCaptureMoves,
	MOVEGEN_GenerateNoMoves
};

static constexpr MG_GenerateMovesFn g_GenerateSpecialMovesFn[COUNT_MOVEMECHANICS] =
{
	MOVEGEN_GenerateNoMoves,
	MOVEGEN_GenerateNoMoves,
	MOVEGEN_GenerateNoMoves,
	MOVEGEN_GenerateNoMoves,
	KING_GenerateCastleMoves
};


void MOVEGEN_GenerateMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, MG_MOVELIST* pMoveList)
{
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	MOVELIST_Initialize(pMoveList);
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		if (!(pPosition->OccupancyPlayerPiece[movingPlayer][piece] & BITBOARD_FULL))
			continue;
		const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][piece];
		g_GenerateQuietMovesFn[pieceInfo.MoveMechanic[MOVETYPE_QUIET]](pMoveGen, pPosition, piece, pMoveList);
		g_GenerateCaptureMovesFn[pieceInfo.MoveMechanic[MOVETYPE_CAPTURE]](pMoveGen, pPosition, piece, pMoveList);
		g_GenerateSpecialMovesFn[pieceInfo.MoveMechanic[MOVETYPE_SPECIAL]](pMoveGen, pPosition, piece, pMoveList);
	}
}

const char* MOVEGEN_GetMoveString(const MG_MOVEGEN* pMoveGen, const MG_PLAYER& player, const MG_MOVE& move)
{
	return pMoveGen->MoveString[player] + ((size_t)move) * MOVESTRING_LENGTH;
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
			if (MOVEGEN_GetMoveString(pMoveGen, player, move)[localPos] == 0)
			{
				break;
			}
			if ((localPos + strPos) >= len)
			{
				found = -1;
			}
			if (MOVEGEN_GetMoveString(pMoveGen, player, move)[localPos] != pString[localPos + strPos])
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
#ifndef NDEBUG
#ifdef MOVEGEN_DEBUG_POSITION
	pOutMoveData->pOldPosition = new MG_POSITION;
	(*pOutMoveData->pOldPosition) = *pPosition;
#endif
#endif
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[movingPlayer][move];
	const MG_PIECETYPE createPiece = moveInfo.CreatePiece;
	const MG_PIECETYPE promoPiece = moveInfo.PromoPiece;
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
	if (createPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (promoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->Header.PassivePlayer;
	pPosition->Header.MoveCount = (tempPlayer == PLAYER_WHITE) ? pPosition->Header.MoveCount + 1 : pPosition->Header.MoveCount;
	pPosition->Header.PassivePlayer = movingPlayer;
	pPosition->Header.MovingPlayer = tempPlayer;
	const MG_CASTLEFLAGS oldFlags = pPosition->Header.CastlingRights;
	const MG_CASTLEFLAGS newFlags = oldFlags & moveInfo.CastleRightsMask;
	const BB_FILEINDEX oldEpFile = pPosition->Header.EpFileIndex;
	pOutMoveData->OldHash = pPosition->Hash;
	pOutMoveData->OldCastlingRights = oldFlags;
	pOutMoveData->OldEnPassantFile = oldEpFile;
	pOutMoveData->OldHalfMoveClock = pPosition->Header.HalfMoveClock;
	pPosition->Header.HalfMoveClock = moveInfo.ResetHalfMoveClock ? 0 : pPosition->Header.HalfMoveClock + 1;
	pPosition->Header.EpFileIndex = moveInfo.EnPassantFileIndex;
	pPosition->Hash ^= HASH_MOVINGPLAYER_BLACK;
	pPosition->Hash ^= moveInfo.HashDelta;
	pPosition->Hash ^= HASH_CastleRights(oldFlags) ^ HASH_CastleRights(newFlags);
	pPosition->Hash ^= HASH_EnPassantFile(oldEpFile) ^ HASH_EnPassantFile(moveInfo.EnPassantFileIndex);
	pPosition->Header.CastlingRights = newFlags;
	MG_DIRTYFLAGS dirtyFlags[COUNT_PLAYERS];
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		dirtyFlags[player] = UINT8_C(0);
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			const bool isDirty = (interestMap & pPosition->InterestPlayerPiece[player][piece]) || ((promoPiece != PIECETYPE_NONE) && (createPiece == piece) && (player == moveInfo.CreatePlayer));
			const std::uint8_t mask = UINT8_C(1) << piece;
			dirtyFlags[player] |= isDirty * mask;
			CM_PREFETCH_COND(isDirty, &pPosition->AttacksPlayerPiece[player][piece]);
		}
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (dirtyFlags[player] & (UINT8_C(1) << piece))
			{
#ifndef MOVEGEN_COMPACT_MOVEDATA
				pOutMoveData->AttacksByPlayerPiece[player][piece] = pPosition->AttacksPlayerPiece[player][piece];
				pOutMoveData->InterestByPlayerPiece[player][piece] = pPosition->InterestPlayerPiece[player][piece];
#endif
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
#ifndef MOVEGEN_COMPACT_MOVEDATA
		pOutMoveData->AttacksByPlayer[player] = pPosition->AttacksPlayer[player];
#endif
		if (attacks != pPosition->AttacksPlayer[player])
			pPosition->AttacksPlayer[player] = attacks;
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		pOutMoveData->DirtyFlags[player] = dirtyFlags[player];
	}
}

void MOVEGEN_UnmakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_MOVEDATA* pMoveData, MG_POSITION* pPosition)
{
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->Header.PassivePlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->Header.PassivePlayer;
	pPosition->Header.PassivePlayer = pPosition->Header.MovingPlayer;
	pPosition->Header.MovingPlayer = tempPlayer;
	pPosition->Hash = pMoveData->OldHash;
	pPosition->Header.CastlingRights = pMoveData->OldCastlingRights;
	pPosition->Header.EpFileIndex = pMoveData->OldEnPassantFile;
	pPosition->Header.HalfMoveClock = pMoveData->OldHalfMoveClock;
	pPosition->Header.MoveCount = (tempPlayer == PLAYER_BLACK) ? pPosition->Header.MoveCount - 1 : pPosition->Header.MoveCount;
	MG_DIRTYFLAGS dirtyFlags[COUNT_PLAYERS];
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		dirtyFlags[player] = pMoveData->DirtyFlags[player];
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
#ifdef MOVEGEN_COMPACT_MOVEDATA
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		BB_BITBOARD pieceAttacks[COUNT_PIECETYPES];
#endif
		std::uint64_t dirty = pMoveData->DirtyFlags[player];  // Promote to 64-bit
		std::int8_t piece;
		while (CM_PopLsb(dirty, piece))
		{
#ifdef MOVEGEN_COMPACT_MOVEDATA
			pieceAttacks[piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			pPosition->AttacksPlayerPiece[player][piece] = pieceAttacks[piece];
#else
			pPosition->AttacksPlayerPiece[player][piece] = pMoveData->AttacksByPlayerPiece[player][piece];
			pPosition->InterestPlayerPiece[player][piece] = pMoveData->InterestByPlayerPiece[player][piece];
#endif
		}
#ifdef MOVEGEN_COMPACT_MOVEDATA
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			attacks |= (dirtyFlags[player] & (UINT8_C(1) << piece)) ? pieceAttacks[piece] : pPosition->AttacksPlayerPiece[player][piece];
		}
#endif
#ifndef MOVEGEN_COMPACT_MOVEDATA
		pPosition->AttacksPlayer[player] = pMoveData->AttacksByPlayer[player];
#else
		if (attacks != pPosition->AttacksPlayer[player])
			pPosition->AttacksPlayer[player] = attacks;
#endif
	}
#ifndef NDEBUG
#ifdef MOVEGEN_DEBUG_POSITION
	ASSERT(POSITION_Equals(pPosition, pMoveData->pOldPosition));
	delete pMoveData->pOldPosition;
#endif
#endif
}

void MOVEGEN_MakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, MG_TENTATIVEMOVEDATA* pOutMoveData, MG_POSITION* pPosition)
{
#ifndef NDEBUG
#ifdef MOVEGEN_DEBUG_POSITION
	pOutMoveData->pOldPosition = new MG_POSITION;
	(*pOutMoveData->pOldPosition) = *pPosition;
#endif
#endif
	BB_BITBOARD interestMap = BITBOARD_EMPTY;
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[movingPlayer][move];
	CM_PREFETCH(&moveInfo);
	const MG_PIECETYPE createPiece = moveInfo.CreatePiece;
	const MG_PIECETYPE promoPiece = moveInfo.PromoPiece;
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
	if (createPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
		interestMap |= createMap;
	}
	if (promoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
		interestMap |= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->Header.PassivePlayer;
	pPosition->Header.PassivePlayer = movingPlayer;
	pPosition->Header.MovingPlayer = tempPlayer;
	pOutMoveData->OldEnPassantFile = pPosition->Header.EpFileIndex;
	pPosition->Header.EpFileIndex = moveInfo.EnPassantFileIndex;
	MG_DIRTYFLAGS dirtyFlags[COUNT_PLAYERS];
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		dirtyFlags[player] = UINT8_C(0);
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			const bool isDirty = (interestMap & pPosition->InterestPlayerPiece[player][piece]) || ((promoPiece != PIECETYPE_NONE) && (createPiece == piece) && (player == moveInfo.CreatePlayer));
			const std::uint8_t mask = UINT8_C(1) << piece;
			dirtyFlags[player] |= isDirty * mask;
			CM_PREFETCH_COND(isDirty, &pPosition->AttacksPlayerPiece[player][piece]);
		}
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (dirtyFlags[player] & (UINT8_C(1) << piece))
			{
#ifndef MOVEGEN_COMPACT_MOVEDATA
				pOutMoveData->AttacksByPlayerPiece[player][piece] = pPosition->AttacksPlayerPiece[player][piece];
				pOutMoveData->InterestByPlayerPiece[player][piece] = pPosition->InterestPlayerPiece[player][piece];
#endif
				pPosition->AttacksPlayerPiece[player][piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			}
			attacks |= pPosition->AttacksPlayerPiece[player][piece];
		}
#ifndef MOVEGEN_COMPACT_MOVEDATA
		pOutMoveData->AttacksByPlayer[player] = pPosition->AttacksPlayer[player];
#endif
		if (attacks != pPosition->AttacksPlayer[player])
			pPosition->AttacksPlayer[player] = attacks;
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		pOutMoveData->DirtyFlags[player] = dirtyFlags[player];
	}
}

void MOVEGEN_UnmakeTentativeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_TENTATIVEMOVEDATA* pMoveData, MG_POSITION* pPosition)
{
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->Header.PassivePlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal ^= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] ^= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] ^= createMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal ^= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] ^= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] ^= killMap;
	}
	if (moveInfo.PromoPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD promoMap = MOVEINFO_GetPromoMap(&moveInfo);
		pPosition->OccupancyTotal ^= promoMap;
		pPosition->OccupancyPlayer[moveInfo.PromoPlayer] ^= promoMap;
		pPosition->OccupancyPlayerPiece[moveInfo.PromoPlayer][moveInfo.PromoPiece] ^= promoMap;
	}
	MG_PLAYER tempPlayer = pPosition->Header.PassivePlayer;
	pPosition->Header.PassivePlayer = pPosition->Header.MovingPlayer;
	pPosition->Header.MovingPlayer = tempPlayer;
	pPosition->Header.EpFileIndex = pMoveData->OldEnPassantFile;
	MG_DIRTYFLAGS dirtyFlags[COUNT_PLAYERS];
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		dirtyFlags[player] = pMoveData->DirtyFlags[player];
	}
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
#ifdef MOVEGEN_COMPACT_MOVEDATA
		BB_BITBOARD attacks = BITBOARD_EMPTY;
		BB_BITBOARD pieceAttacks[COUNT_PIECETYPES];
#endif
		std::uint64_t dirty = pMoveData->DirtyFlags[player];  // Promote to 64-bit
		std::int8_t piece;
		while (CM_PopLsb(dirty, piece))
		{
#ifdef MOVEGEN_COMPACT_MOVEDATA
			pieceAttacks[piece] = MOVEGEN_GetPieceAttacks(pMoveGen, pPosition, piece, player, pPosition->InterestPlayerPiece[player][piece]);
			pPosition->AttacksPlayerPiece[player][piece] = pieceAttacks[piece];
#else
			pPosition->AttacksPlayerPiece[player][piece] = pMoveData->AttacksByPlayerPiece[player][piece];
			pPosition->InterestPlayerPiece[player][piece] = pMoveData->InterestByPlayerPiece[player][piece];
#endif
		}
#ifdef MOVEGEN_COMPACT_MOVEDATA
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			attacks |= (dirtyFlags[player] & (UINT8_C(1) << piece)) ? pieceAttacks[piece] : pPosition->AttacksPlayerPiece[player][piece];
		}
#endif
#ifndef MOVEGEN_COMPACT_MOVEDATA
		pPosition->AttacksPlayer[player] = pMoveData->AttacksByPlayer[player];
#else
		if (attacks != pPosition->AttacksPlayer[player])
			pPosition->AttacksPlayer[player] = attacks;
#endif
	}
#ifndef NDEBUG
#ifdef MOVEGEN_DEBUG_POSITION
	ASSERT(POSITION_Equals(pPosition, pMoveData->pOldPosition));
	delete pMoveData->pOldPosition;
#endif
#endif
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
#ifdef MOVEGEN_LEGAL
	MG_TENTATIVEMOVEDATA moveData;
	MOVEGEN_MakeTentativeMove(pMoveGen, move, &moveData, pPosition);
	const bool bLegal = POSITION_IsLegal(pPosition);
	MOVEGEN_UnmakeTentativeMove(pMoveGen, move, &moveData, pPosition);
	if (bLegal)
		pMoveList->Move[pMoveList->CountMoves++] = move;
#else
	pMoveList->Move[pMoveList->CountMoves++] = move;
#endif
}

std::uint64_t MOVEGEN_Perft(MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen, int depth, std::uint64_t& nodeCount)
{
	if (depth <= 0)
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
#ifdef MOVEGEN_LEGAL
		nodeCount++;
		if (depth == 1)
		{
			nodes++;
		}
		else
		{
			nodes += MOVEGEN_Perft(pPosition, pMoveGen, depth - 1, nodeCount);
		}
#else
		if (POSITION_IsLegal(pPosition))
		{
			nodeCount++;
			if (depth == 1)
			{
				nodes++;
			}
			else
			{
				nodes += MOVEGEN_Perft(pPosition, pMoveGen, depth - 1, nodeCount);
			}
		}
#endif
		MOVEGEN_UnmakeMove(pMoveGen, move, &moveData, pPosition);
	}
	return nodes;
}