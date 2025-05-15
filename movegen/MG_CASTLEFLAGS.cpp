#include "MG_CASTLEFLAGS.h"
#include "libCommon.h"


bool CASTLEFLAGS_Parse(const char* pString, const int& len, int& strPos, MG_CASTLEFLAGS& outCastleFlags)
{
	if (strPos >= len)
		return false;
	outCastleFlags = CASTLEFLAGS_NONE;
	if (pString[strPos] == '-')
	{
		strPos++;
		return true;
	}
	int idx;
	for (idx = 0; idx < 4; idx++)
	{
		bool bBreakOut = false;
		if (strPos >= len)
			break;
		switch (pString[strPos])
		{
		default:
			bBreakOut = true;
			break;
		case 'K':
			outCastleFlags |= CASTLEFLAGS_WHITE_KINGSIDE;
			strPos++;
			break;
		case 'Q':
			outCastleFlags |= CASTLEFLAGS_WHITE_QUEENSIDE;
			strPos++;
			break;
		case 'k':
			outCastleFlags |= CASTLEFLAGS_BLACK_KINGSIDE;
			strPos++;
			break;
		case 'q':
			outCastleFlags |= CASTLEFLAGS_BLACK_QUEENSIDE;
			strPos++;
			break;
		}
		if (bBreakOut)
			break;
	}
	return idx > 0;
}

bool CASTLEFLAGS_ToString(char* pString, const int& len, int& strPos, const MG_CASTLEFLAGS& castleFlags)
{
	if (strPos >= len)
		return false;
	if (castleFlags == CASTLEFLAGS_NONE)
	{
		pString[strPos++] = '-';
		return true;
	}
	if (castleFlags & CASTLEFLAGS_WHITE_KINGSIDE)
	{
		pString[strPos++] = 'K';
	}
	if (castleFlags & CASTLEFLAGS_WHITE_QUEENSIDE)
	{
		pString[strPos++] = 'Q';
	}
	if (castleFlags & CASTLEFLAGS_BLACK_KINGSIDE)
	{
		pString[strPos++] = 'k';
	}
	if (castleFlags & CASTLEFLAGS_BLACK_QUEENSIDE)
	{
		pString[strPos++] = 'q';
	}
	return true;
}

MG_CASTLEFLAGS CASTLEFLAGS_EliminateFlags_Move(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare)
{
	if (movingPiece == PIECETYPE_ROOK)
	{
		if (movingPlayer == PLAYER_WHITE)
		{
			if (fromSquare == SQUARE_H1)
				return CASTLEFLAGS_WHITE_KINGSIDE;
			else if (fromSquare == SQUARE_A1)
				return CASTLEFLAGS_WHITE_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
		else
		{
			if (fromSquare == SQUARE_H8)
				return CASTLEFLAGS_BLACK_KINGSIDE;
			else if (fromSquare == SQUARE_A8)
				return CASTLEFLAGS_BLACK_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
	}
	else if (movingPiece == PIECETYPE_KING)
	{
		if (movingPlayer == PLAYER_WHITE)
			return CASTLEFLAGS_WHITE_KINGSIDE | CASTLEFLAGS_WHITE_QUEENSIDE;
		else
			return CASTLEFLAGS_BLACK_KINGSIDE | CASTLEFLAGS_BLACK_QUEENSIDE;
	}
	else
		return CASTLEFLAGS_NONE;
}

MG_CASTLEFLAGS CASTLEFLAGS_EliminateFlags_Capture(const MG_PLAYER& movingPlayer, const MG_PIECETYPE& movingPiece, const BB_SQUARE& fromSquare, const BB_SQUARE& toSquare, const MG_PIECETYPE& capturedPiece)
{
	if (capturedPiece == PIECETYPE_ROOK)
	{
		if (movingPlayer == PLAYER_BLACK)
		{
			if (toSquare == SQUARE_H1)
				return CASTLEFLAGS_WHITE_KINGSIDE;
			else if (toSquare == SQUARE_A1)
				return CASTLEFLAGS_WHITE_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
		else
		{
			if (toSquare == SQUARE_H8)
				return CASTLEFLAGS_BLACK_KINGSIDE;
			else if (toSquare == SQUARE_A8)
				return CASTLEFLAGS_BLACK_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
	}
	if (movingPiece == PIECETYPE_ROOK)
	{
		if (movingPlayer == PLAYER_WHITE)
		{
			if (fromSquare == SQUARE_H1)
				return CASTLEFLAGS_WHITE_KINGSIDE;
			else if (fromSquare == SQUARE_A1)
				return CASTLEFLAGS_WHITE_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
		else
		{
			if (fromSquare == SQUARE_H8)
				return CASTLEFLAGS_BLACK_KINGSIDE;
			else if (fromSquare == SQUARE_A8)
				return CASTLEFLAGS_BLACK_QUEENSIDE;
			else
				return CASTLEFLAGS_NONE;
		}
	}
	else if (movingPiece == PIECETYPE_KING)
	{
		if (movingPlayer == PLAYER_WHITE)
			return CASTLEFLAGS_WHITE_KINGSIDE | CASTLEFLAGS_WHITE_QUEENSIDE;
		else
			return CASTLEFLAGS_BLACK_KINGSIDE | CASTLEFLAGS_BLACK_QUEENSIDE;
	}
	else
		return CASTLEFLAGS_NONE;
}
