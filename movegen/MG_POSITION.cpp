#include "MG_POSITION.h"

void POSITION_Clear(MG_POSITION* pPosition)
{
	pPosition->OccupancyTotal = BITBOARD_EMPTY;
	for (MG_PLAYER player = 0; player < COUNT_PLAYERS; player++)
	{
		pPosition->OccupancyPlayer[player] = BITBOARD_EMPTY;
		for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
		{
			pPosition->OccupancyPlayerPiece[player][piece] = BITBOARD_EMPTY;
		}
	}
	pPosition->MovingPlayer = PLAYER_WHITE;
	pPosition->PassivePlayer = PLAYER_BLACK;
	pPosition->CastlingRights = CASTLEFLAGS_NONE;
	pPosition->Hash = HASH_CastleRights(CASTLEFLAGS_NONE);
}

void POSITION_SetCastleRights(MG_POSITION* pPosition, const MG_CASTLEFLAGS& castlingRights)
{
	const MG_CASTLEFLAGS oldFlags = pPosition->CastlingRights;
	pPosition->CastlingRights = castlingRights;
	pPosition->Hash ^= HASH_CastleRights(oldFlags) ^ HASH_CastleRights(castlingRights);
}


void POSITION_SetPiece(MG_POSITION* pPosition, const MG_PLAYER& player, const MG_PIECETYPE& piece, const BB_SQUARE& square)
{
	pPosition->OccupancyPlayer[player] |= square;
	pPosition->OccupancyPlayerPiece[player][piece] |= square;
	pPosition->OccupancyTotal |= square;
	pPosition->Hash ^= HASH_PlayerPieceSquare(player, piece, square);
}

void POSITION_Initialize(MG_POSITION* pPosition)
{
	POSITION_Clear(pPosition);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_KING, SQUARE_E1);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_KING, SQUARE_E8);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_ROOK, SQUARE_A1);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_ROOK, SQUARE_H1);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_ROOK, SQUARE_A8);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_ROOK, SQUARE_H8);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_KNIGHT, SQUARE_B1);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_KNIGHT, SQUARE_G1);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_KNIGHT, SQUARE_B8);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_KNIGHT, SQUARE_G8);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_BISHOP, SQUARE_C1);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_BISHOP, SQUARE_F1);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_BISHOP, SQUARE_C8);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_BISHOP, SQUARE_F8);
	POSITION_SetPiece(pPosition, PLAYER_WHITE, PIECETYPE_QUEEN, SQUARE_D1);
	POSITION_SetPiece(pPosition, PLAYER_BLACK, PIECETYPE_QUEEN, SQUARE_D8);
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
	return hash;
}

