#include "MG_JUMPTABLE.h"
#include "MG_MOVEGEN.h"
#include "MG_MOVELIST.h"
#include "MG_POSITION.h"
#include "libCommon.h"


void JUMPTABLE_Initialize_QuietMoves(const MG_PLAYER& player, const MG_PIECETYPE& piece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable)
{
	pMoveGen->JumpTable[jumptable].TargetIndex = jumptarget;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = pMoveGen->JumpTargets[jumptarget][squareIndexFrom];
		pMoveGen->JumpTable[jumptable].MovesBaseFrom[squareIndexFrom] = nextMove;
		BB_SQUARECOUNT count = BITBOARD_PopulationCount(targets);
		for (BB_SQUARECOUNT squareBit = 0; squareBit < count; squareBit++)
		{
			BB_BITBOARD squareBitIndex = UINT64_C(1) << squareBit;
			const BB_SQUARE squareTo = BITBOARD_BitDeposit(squareBitIndex, targets);
			const MG_MOVE move = nextMove++;
			const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
			ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
#endif
			pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
			pMoveGen->MoveTable[player][move].KillPiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].KillPlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].KillDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].MovePiece = piece;
			pMoveGen->MoveTable[player][move].MovePlayer = player;
			pMoveGen->MoveTable[player][move].HashDelta = HASH_PlayerPieceSquare(player, piece, squareFrom) ^ HASH_PlayerPieceSquare(player, piece, squareTo);
			pMoveGen->MoveTable[player][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(player, piece, squareFrom, squareTo);
			MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
		}
	}
}

void JUMPTABLE_Initialize_CaptureMoves(const MG_PLAYER& player, const MG_PIECETYPE& movingPiece, const MG_PIECETYPE& capturedPiece, MG_MOVEGEN* pMoveGen, MG_MOVE& nextMove, const int& jumptarget, const int& jumptable)
{
	const MG_PLAYER otherPlayer = PLAYER_OTHER(player);
	pMoveGen->JumpTable[jumptable].TargetIndex = jumptarget;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = pMoveGen->JumpTargets[jumptarget][squareIndexFrom];
		pMoveGen->JumpTable[jumptable].MovesBaseFrom[squareIndexFrom] = nextMove;
		BB_SQUARECOUNT count = BITBOARD_PopulationCount(targets);
		for (BB_SQUARECOUNT squareBit = 0; squareBit < count; squareBit++)
		{
			BB_BITBOARD squareBitIndex = UINT64_C(1) << squareBit;
			const BB_SQUARE squareTo = BITBOARD_BitDeposit(squareBitIndex, targets);
			const MG_MOVE move = nextMove++;
			const BB_SQUAREINDEX squareIndexTo = SQUARE_GetIndex(squareTo);
			ASSERT(move < pMoveGen->CountMoves);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = squareTo;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
#endif
			pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
			pMoveGen->MoveTable[player][move].KillPiece = capturedPiece;
			pMoveGen->MoveTable[player][move].KillPlayer = otherPlayer;
			pMoveGen->MoveTable[player][move].KillDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MovePiece = movingPiece;
			pMoveGen->MoveTable[player][move].MovePlayer = player;
			pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].HashDelta = HASH_PlayerPieceSquare(player, movingPiece, squareFrom) ^ HASH_PlayerPieceSquare(player, movingPiece, squareTo) ^ HASH_PlayerPieceSquare(otherPlayer, capturedPiece, squareTo);
			pMoveGen->MoveTable[player][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(player, movingPiece, squareFrom, squareTo, capturedPiece);
			MOVEINFO_InitializeMoveString(pMoveGen->MoveTable[player][move].MoveString, squareFrom, squareTo);
		}
	}
}

MG_MOVE JUMPTABLE_CountMoves(const MG_MOVEGEN* pMoveGen, const int& jumptarget)
{
	MG_MOVE count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_SQUARE squareFrom = SQUARE_FromIndex(squareIndexFrom);
		const BB_BITBOARD targets = pMoveGen->JumpTargets[jumptarget][squareIndexFrom];
		BB_SQUARECOUNT countTargets = BITBOARD_PopulationCount(targets);
		count += ((MG_MOVE)countTargets) * (1 + COUNT_PIECETYPES);
	}
	return count;
}


void JUMPTABLE_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_QUIET];
	const MG_JUMPTABLE& table = pMoveGen->JumpTable[tableIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		const BB_BITBOARD targets = pMoveGen->JumpTargets[table.TargetIndex][fromSquareIndex];
		BB_BITBOARD destinations = targets & ~pPosition->OccupancyTotal;
		BB_SQUAREINDEX toSquareIndex;
		while (SQUARE_Next(destinations, toSquareIndex))
		{
			const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
			const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, targets);
			const MG_MOVE move = table.MovesBaseFrom[fromSquareIndex] + optionIndex;
			pMoveList->Move[pMoveList->CountMoves++] = move;
		}
	}
}


void JUMPTABLE_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_CAPTURE(piece)];
	const MG_JUMPTABLE& table = pMoveGen->JumpTable[tableIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		const BB_BITBOARD targets = pMoveGen->JumpTargets[table.TargetIndex][fromSquareIndex];
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			BB_BITBOARD destinations = targets & pPosition->OccupancyPlayerPiece[pPosition->PassivePlayer][capturedPiece];
			BB_SQUAREINDEX toSquareIndex;
			while (SQUARE_Next(destinations, toSquareIndex))
			{
				const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
				const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, targets);
				const MG_MOVE move = table.MovesBaseFrom[fromSquareIndex] + optionIndex;
				pMoveList->Move[pMoveList->CountMoves++] = move;
			}
		}
	}
}
