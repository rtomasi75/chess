#include "MG_PIECETYPE.h"
#include "libCommon.h"

bool PIECETYPE_Parse(const char* pString, const int& len, int& strPos, MG_PIECETYPE& outParsedPiece, MG_PLAYER& outParsedPlayer)
{
	if (strPos >= len)
		return false;
	switch (pString[strPos])
	{
	default:
		return false;
	case 'k':
		outParsedPiece = PIECETYPE_KING;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'n':
		outParsedPiece = PIECETYPE_KNIGHT;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'r':
		outParsedPiece = PIECETYPE_ROOK;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'b':
		outParsedPiece = PIECETYPE_BISHOP;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'q':
		outParsedPiece = PIECETYPE_QUEEN;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'p':
		outParsedPiece = PIECETYPE_PAWN;
		outParsedPlayer = PLAYER_BLACK;
		break;
	case 'K':
		outParsedPiece = PIECETYPE_KING;
		outParsedPlayer = PLAYER_WHITE;
		break;
	case 'N':
		outParsedPiece = PIECETYPE_KNIGHT;
		outParsedPlayer = PLAYER_WHITE;
		break;
	case 'R':
		outParsedPiece = PIECETYPE_ROOK;
		outParsedPlayer = PLAYER_WHITE;
		break;
	case 'B':
		outParsedPiece = PIECETYPE_BISHOP;
		outParsedPlayer = PLAYER_WHITE;
		break;
	case 'Q':
		outParsedPiece = PIECETYPE_QUEEN;
		outParsedPlayer = PLAYER_WHITE;
		break;
	case 'P':
		outParsedPiece = PIECETYPE_PAWN;
		outParsedPlayer = PLAYER_WHITE;
		break;
	}
	strPos++;
	return true;
}

bool PIECETYPE_ToString(char* pString, const int& len, int& strPos, const MG_PIECETYPE& piece, const MG_PLAYER& player)
{
	if (strPos >= len)
		return false;
	if (player == PLAYER_WHITE)
	{
		switch (piece)
		{
		default:
			ASSERT(false);
			break;
		case PIECETYPE_KING:
			pString[strPos] = 'K';
			break;
		case PIECETYPE_KNIGHT:
			pString[strPos] = 'N';
			break;
		case PIECETYPE_ROOK:
			pString[strPos] = 'R';
			break;
		case PIECETYPE_BISHOP:
			pString[strPos] = 'B';
			break;
		case PIECETYPE_QUEEN:
			pString[strPos] = 'Q';
			break;
		case PIECETYPE_PAWN:
			pString[strPos] = 'P';
			break;
		case PIECETYPE_NONE:
			pString[strPos] = '.';
			break;
		}
	}
	else
	{
		switch (piece)
		{
		default:
			ASSERT(false);
			break;
		case PIECETYPE_KING:
			pString[strPos] = 'k';
			break;
		case PIECETYPE_KNIGHT:
			pString[strPos] = 'n';
			break;
		case PIECETYPE_ROOK:
			pString[strPos] = 'r';
			break;
		case PIECETYPE_BISHOP:
			pString[strPos] = 'b';
			break;
		case PIECETYPE_QUEEN:
			pString[strPos] = 'q';
			break;
		case PIECETYPE_PAWN:
			pString[strPos] = 'p';
			break;
		case PIECETYPE_NONE:
			pString[strPos] = '.';
			break;
		}
	}
	strPos++;
	return true;
}
