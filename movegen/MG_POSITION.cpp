#include "MG_POSITION.h"
#include "MG_MOVEGEN.h"
#include <cstdio>

void POSITION_Clear(MG_POSITION* pPosition)
{
	pPosition->OccupancyTotal = BITBOARD_EMPTY;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		pPosition->OccupancyPlayer[player] = BITBOARD_EMPTY;
		pPosition->AttacksPlayer[player] = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			pPosition->OccupancyPlayerPiece[player][piece] = BITBOARD_EMPTY;
			pPosition->AttacksPlayerPiece[player][piece] = BITBOARD_EMPTY;
			pPosition->InterestPlayerPiece[player][piece] = BITBOARD_EMPTY;
		}
	}
	pPosition->MovingPlayer = PLAYER_WHITE;
	pPosition->PassivePlayer = PLAYER_BLACK;
	pPosition->CastlingRights = CASTLEFLAGS_NONE;
	pPosition->EpFileIndex = FILEINDEX_NONE;
	pPosition->MoveCount = 1;
	pPosition->HalfMoveClock = 0;
	pPosition->Hash = HASH_CastleRights(CASTLEFLAGS_NONE);
}

void POSITION_SetCastleRights(MG_POSITION* pPosition, const MG_CASTLEFLAGS& castlingRights)
{
	const MG_CASTLEFLAGS oldFlags = pPosition->CastlingRights;
	pPosition->CastlingRights = castlingRights;
	pPosition->Hash ^= HASH_CastleRights(oldFlags) ^ HASH_CastleRights(castlingRights);
}


void POSITION_SetPiece(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& square)
{
	pPosition->OccupancyPlayer[player] |= square;
	pPosition->OccupancyPlayerPiece[player][piece] |= square;
	pPosition->OccupancyTotal |= square;
	pPosition->Hash ^= HASH_PlayerPieceSquare(player, piece, square);
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
		MOVEGEN_RecomputeAttacks(pMoveGen, pPosition, player);
}

void POSITION_Initialize(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition)
{
	POSITION_Clear(pPosition);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_KING, SQUARE_E1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_KING, SQUARE_E8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_ROOK, SQUARE_A1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_ROOK, SQUARE_H1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_ROOK, SQUARE_A8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_ROOK, SQUARE_H8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_KNIGHT, SQUARE_B1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_KNIGHT, SQUARE_G1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_KNIGHT, SQUARE_B8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_KNIGHT, SQUARE_G8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_BISHOP, SQUARE_C1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_BISHOP, SQUARE_F1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_BISHOP, SQUARE_C8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_BISHOP, SQUARE_F8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_QUEEN, SQUARE_D1);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_QUEEN, SQUARE_D8);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_A2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_B2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_C2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_D2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_E2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_F2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_G2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_WHITE, PIECETYPE_PAWN, SQUARE_H2);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_A7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_B7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_C7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_D7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_E7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_F7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_G7);
	POSITION_SetPiece(pMoveGen, pPosition, PLAYER_BLACK, PIECETYPE_PAWN, SQUARE_H7);
	POSITION_SetCastleRights(pPosition, CASTLEFLAGS_BLACK_KINGSIDE | CASTLEFLAGS_BLACK_QUEENSIDE | CASTLEFLAGS_WHITE_KINGSIDE | CASTLEFLAGS_WHITE_QUEENSIDE);
}

bool POSITION_GetPiece(const MG_POSITION* pPosition, const BB_SQUARE& square, MG_PLAYER& outPlayer, MG_PIECETYPE& outPiece)
{
	if (pPosition->OccupancyTotal & square)
	{
		if (pPosition->OccupancyPlayer[PLAYER_WHITE] & square)
			outPlayer = PLAYER_WHITE;
		else
			outPlayer = PLAYER_BLACK;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (pPosition->OccupancyPlayerPiece[outPlayer][piece] & square)
			{
				outPiece = piece;
				return true;
			}
		}
	}
	return false;
}

bool POSITION_CheckConsistency(const MG_POSITION* pPosition, const BB_SQUARE& square)
{
	if (pPosition->OccupancyTotal & square)
	{
		MG_PLAYER player;
		MG_PLAYER other;
		if (pPosition->OccupancyPlayer[PLAYER_WHITE] & square)
		{
			player = PLAYER_WHITE;
			other = PLAYER_BLACK;
		}
		else
		{
			player = PLAYER_BLACK;
			other = PLAYER_WHITE;
		}
		if (!(pPosition->OccupancyPlayer[player] & square))
			return false;
		if (pPosition->OccupancyPlayer[other] & square)
			return false;
		for (MG_PLAYER pl = 0; pl < COUNT_PLAYERS; pl++)
		{
			int count = 0;
			for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
			{
				if (pPosition->OccupancyPlayerPiece[pl][piece] & square)
					count++;
			}
			if (pl == player)
			{
				if (count != 1)
					return false;
			}
			else
			{
				if (count != 0)
					return false;
			}
		}
	}
	else
	{
		if (pPosition->OccupancyPlayer[PLAYER_WHITE] & square)
			return false;
		if (pPosition->OccupancyPlayer[PLAYER_BLACK] & square)
			return false;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (pPosition->OccupancyPlayerPiece[PLAYER_WHITE][piece] & square)
				return false;
			if (pPosition->OccupancyPlayerPiece[PLAYER_BLACK][piece] & square)
				return false;
		}
	}
	return true;
}

MG_HASH POSITION_ComputeHash(const MG_POSITION* pPosition)
{
	MG_HASH hash = pPosition->MovingPlayer == PLAYER_BLACK ? HASH_MOVINGPLAYER_BLACK : HASH_EMPTY;
	hash ^= HASH_CastleRights(pPosition->CastlingRights);
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			BB_BITBOARD occupancy = pPosition->OccupancyPlayerPiece[player][piece];
			BB_SQUAREINDEX squareIndex;
			while (SQUARE_Next(occupancy, squareIndex))
			{
				hash ^= HASH_PlayerPieceSquare(player, piece, SQUARE_FromIndex(squareIndex));
			}
		}
	}
	hash ^= HASH_EnPassantFile(pPosition->EpFileIndex);
	return hash;
}

bool POSITION_WriteEmptySpaceCounter(char* pString, const int& len, int& strPos, const int& emptySpaceCounter)
{
	switch (emptySpaceCounter)
	{
	case 0:
		return true;
	case 1:
		if (strPos >= len)
			return false;
		pString[strPos++] = '1';
		return true;
	case 2:
		if (strPos >= len)
			return false;
		pString[strPos++] = '2';
		return true;
	case 3:
		if (strPos >= len)
			return false;
		pString[strPos++] = '3';
		return true;
	case 4:
		if (strPos >= len)
			return false;
		pString[strPos++] = '4';
		return true;
	case 5:
		if (strPos >= len)
			return false;
		pString[strPos++] = '5';
		return true;
	case 6:
		if (strPos >= len)
			return false;
		pString[strPos++] = '6';
		return true;
	case 7:
		if (strPos >= len)
			return false;
		pString[strPos++] = '7';
		return true;
	case 8:
		if (strPos >= len)
			return false;
		pString[strPos++] = '8';
		return true;
	default:
		ASSERT(false);
		return false;
	}
}

void POSITION_SetEnPassantFile(MG_POSITION* pPosition, const BB_FILEINDEX& epFileIdx)
{
	pPosition->Hash ^= HASH_EnPassantFile(pPosition->EpFileIndex);
	pPosition->EpFileIndex = epFileIdx;
	pPosition->Hash ^= HASH_EnPassantFile(pPosition->EpFileIndex);
}

void POSITION_SetMovingPlayer(MG_POSITION* pPosition, const MG_PLAYER& movingPlayer)
{
	if (pPosition->MovingPlayer != movingPlayer)
	{
		pPosition->MovingPlayer = movingPlayer;
		pPosition->PassivePlayer = (movingPlayer == PLAYER_WHITE) ? PLAYER_BLACK : PLAYER_WHITE;
		pPosition->Hash ^= HASH_MOVINGPLAYER_BLACK;
	}
}

bool POSITION_ToString(char* pString, const int& len, int& strPos, const MG_POSITION& position)
{
	for (BB_RANKINDEX rankIdx = COUNT_RANKS - 1; rankIdx >= 0; rankIdx--)
	{
		int emptySpaceCounter = 0;
		for (BB_FILEINDEX fileIdx = 0; fileIdx < COUNT_RANKS; fileIdx++)
		{
			const BB_SQUARE square = SQUARE_FromRankFileIndices(rankIdx, fileIdx);
			MG_PLAYER player;
			MG_PIECETYPE piece;
			if (POSITION_GetPiece(&position, square, player, piece))
			{
				if (!POSITION_WriteEmptySpaceCounter(pString, len, strPos, emptySpaceCounter))
					return false;
				emptySpaceCounter = 0;
				if (!PIECETYPE_ToString(pString, len, strPos, piece, player))
					return false;
			}
			else
				emptySpaceCounter++;
		}
		if (!POSITION_WriteEmptySpaceCounter(pString, len, strPos, emptySpaceCounter))
			return false;
		if (rankIdx != 0)
		{
			if (strPos >= len)
				return false;
			pString[strPos++] = '/';
		}
	}
	if (strPos >= len)
		return false;
	pString[strPos++] = ' ';
	if (!PLAYER_ToString(pString, len, strPos, position.MovingPlayer))
		return false;
	if (strPos >= len)
		return false;
	pString[strPos++] = ' ';
	if (!CASTLEFLAGS_ToString(pString, len, strPos, position.CastlingRights))
		return false;
	if (strPos >= len)
		return false;
	pString[strPos++] = ' ';
	if (position.EpFileIndex == FILEINDEX_NONE)
	{
		if (strPos >= len)
			return false;
		pString[strPos++] = '-';
	}
	else
	{
		const BB_FILE epFile = FILE_FromIndex(position.EpFileIndex);
		const BB_RANK epRank = (position.MovingPlayer == PLAYER_WHITE) ? RANK_3 : RANK_6;
		const BB_SQUARE epSquare = SQUARE_FromRankFile(epRank, epFile);
		if (!SQUARE_ToString(pString, len, strPos, epSquare))
			return false;
	}
	pString[strPos++] = ' ';
	if (strPos >= len)
		return false;
	char buffer[10];
	int l = sprintf_s(buffer, "%d", position.HalfMoveClock);
	if ((strPos + l) >= len)
		return false;
	memcpy(pString + strPos, buffer, l);
	strPos += l;
	if (strPos >= len)
		return false;
	pString[strPos++] = ' ';
	if (strPos >= len)
		return false;
	l = sprintf_s(buffer, "%d", position.MoveCount);
	if ((strPos + l) > len)
		return false;
	memcpy(pString + strPos, buffer, l);
	strPos += l;
	return true;
}

bool POSITION_Equals(const MG_POSITION* pPosition1, const MG_POSITION* pPosition2)
{
	if (pPosition1->Hash != pPosition2->Hash)
		return false;
	if (pPosition1->MovingPlayer != pPosition2->MovingPlayer)
		return false;
	if (pPosition1->PassivePlayer != pPosition2->PassivePlayer)
		return false;
	if (pPosition1->CastlingRights != pPosition2->CastlingRights)
		return false;
	if (pPosition1->EpFileIndex != pPosition2->EpFileIndex)
		return false;
	if (pPosition1->OccupancyTotal != pPosition2->OccupancyTotal)
		return false;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		if (pPosition1->OccupancyPlayer[player] != pPosition2->OccupancyPlayer[player])
			return false;
		if (pPosition1->AttacksPlayer[player] != pPosition2->AttacksPlayer[player])
			return false;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			if (pPosition1->OccupancyPlayerPiece[player][piece] != pPosition2->OccupancyPlayerPiece[player][piece])
				return false;
			if (pPosition1->InterestPlayerPiece[player][piece] != pPosition2->InterestPlayerPiece[player][piece])
				return false;
			if (pPosition1->InterestPlayerPiece[player][piece] != pPosition2->InterestPlayerPiece[player][piece])
				return false;
		}
	}
	return true;
}

bool POSITION_Parse(const MG_MOVEGEN* pMoveGen, const char* pString, const int& len, int& strPos, MG_POSITION& outParsed)
{
	POSITION_Clear(&outParsed);
	BB_RANKINDEX rankIdx = COUNT_RANKS - 1;
	BB_FILEINDEX fileIdx = 0;
	MG_PLAYER player;
	MG_PIECETYPE piece;
	while (rankIdx >= 0)
	{
		if (strPos >= len)
			return false;
		switch (pString[strPos])
		{
		case '1':
			fileIdx += 1;
			strPos++;
			break;
		case '2':
			fileIdx += 2;
			strPos++;
			break;
		case '3':
			fileIdx += 3;
			strPos++;
			break;
		case '4':
			fileIdx += 4;
			strPos++;
			break;
		case '5':
			fileIdx += 5;
			strPos++;
			break;
		case '6':
			fileIdx += 6;
			strPos++;
			break;
		case '7':
			fileIdx += 7;
			strPos++;
			break;
		case '8':
			fileIdx += 8;
			strPos++;
			break;
		default:
			if (PIECETYPE_Parse(pString, len, strPos, piece, player))
			{
				POSITION_SetPiece(pMoveGen, &outParsed, player, piece, SQUARE_FromRankFileIndices(rankIdx, fileIdx));
				fileIdx++;
			}
			else
				return false;
		}
		if (fileIdx >= COUNT_FILES)
		{
			if (rankIdx != 0)
			{
				if (strPos >= len)
					return false;
				if (pString[strPos++] != '/')
					return false;
			}
			fileIdx = 0;
			rankIdx--;
		}
	}
	if (strPos >= len)
		return false;
	if (pString[strPos++] != ' ')
		return false;
	if (!PLAYER_Parse(pString, len, strPos, player))
		return false;
	POSITION_SetMovingPlayer(&outParsed, player);
	if (strPos >= len)
		return false;
	if (pString[strPos++] != ' ')
		return false;
	MG_CASTLEFLAGS castleFlags;
	if (!CASTLEFLAGS_Parse(pString, len, strPos, castleFlags))
		return false;
	POSITION_SetCastleRights(&outParsed, castleFlags);
	if (strPos >= len)
		return false;
	if (pString[strPos++] != ' ')
		return false;
	if (strPos >= len)
		return false;
	if (pString[strPos] == '-')
		strPos++;
	else
	{
		BB_SQUARE epSquare;
		if (!SQUARE_Parse(pString, len, strPos, epSquare))
			return false;
		const BB_FILEINDEX epFileIdx = SQUARE_GetFileIndex(SQUARE_GetIndex(epSquare));
		POSITION_SetEnPassantFile(&outParsed, epFileIdx);
	}
	int value = 0;
	int l = sscanf_s("&d", pString + strPos, value);
	if (l == 0 || l == EOF)
	{
		return false;
	}
	strPos += l;
	outParsed.HalfMoveClock = (MG_HALFMOVECOUNT)value;
	if (strPos >= len)
		return false;
	if (pString[strPos++] != ' ')
		return false;
	l = sscanf_s("&d", pString + strPos, value);
	if (l == 0 || l == EOF)
	{
		return false;
	}
	outParsed.MoveCount = (MG_FULLMOVECOUNT)value;
	return true;
}
