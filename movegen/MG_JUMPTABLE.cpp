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
			ASSERT(move < pMoveGen->CountMoves[player]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].PromoMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
#endif
			pMoveGen->MoveTable[player][move].MoveDest = squareIndexTo;
			pMoveGen->MoveTable[player][move].MoveSource = squareIndexFrom;
			pMoveGen->MoveTable[player][move].KillPiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].KillPlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].KillDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].PromoPiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].PromoPlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].PromoSource = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].CreatePiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].CreatePlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].CreateDest = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].ResetHalfMoveClock = false;
			pMoveGen->MoveTable[player][move].MovePiece = piece;
			pMoveGen->MoveTable[player][move].MovePlayer = player;
			pMoveGen->MoveTable[player][move].EnPassantFileIndex = FILEINDEX_NONE;
			pMoveGen->MoveTable[player][move].HashDelta = HASH_PlayerPieceSquare(player, piece, squareFrom) ^ HASH_PlayerPieceSquare(player, piece, squareTo);
			pMoveGen->MoveTable[player][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Move(player, piece, squareFrom, squareTo);
			MOVEINFO_InitializeMoveString(pMoveGen, player, move, squareFrom, squareTo);
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
			ASSERT(move < pMoveGen->CountMoves[player]);
#ifndef MOVEGEN_COMPACT_MOVEINFO
			pMoveGen->MoveTable[player][move].KillMap = squareTo;
			pMoveGen->MoveTable[player][move].MoveMap = squareFrom ^ squareTo;
			pMoveGen->MoveTable[player][move].CreateMap = BITBOARD_EMPTY;
			pMoveGen->MoveTable[player][move].PromoMap = BITBOARD_EMPTY;
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
			pMoveGen->MoveTable[player][move].ResetHalfMoveClock = true;
			pMoveGen->MoveTable[player][move].PromoPiece = PIECETYPE_NONE;
			pMoveGen->MoveTable[player][move].PromoPlayer = PLAYER_NONE;
			pMoveGen->MoveTable[player][move].PromoSource = SQUAREINDEX_NONE;
			pMoveGen->MoveTable[player][move].EnPassantFileIndex = FILEINDEX_NONE;
			pMoveGen->MoveTable[player][move].HashDelta = HASH_PlayerPieceSquare(player, movingPiece, squareFrom) ^ HASH_PlayerPieceSquare(player, movingPiece, squareTo) ^ HASH_PlayerPieceSquare(otherPlayer, capturedPiece, squareTo);
			pMoveGen->MoveTable[player][move].CastleRightsMask = ~CASTLEFLAGS_EliminateFlags_Capture(player, movingPiece, squareFrom, squareTo, capturedPiece);
			MOVEINFO_InitializeMoveString(pMoveGen, player, move, squareFrom, squareTo);
		}
	}
}

MG_MOVE JUMPTABLE_CountMoves(const MG_MOVEGEN* pMoveGen, const int& jumptarget)
{
	MG_MOVE count = 0;
	for (BB_SQUAREINDEX squareIndexFrom = 0; squareIndexFrom < COUNT_SQUARES; squareIndexFrom++)
	{
		const BB_BITBOARD targets = pMoveGen->JumpTargets[jumptarget][squareIndexFrom];
		BB_SQUARECOUNT countTargets = BITBOARD_PopulationCount(targets);
		count += ((MG_MOVE)countTargets) * (1 + COUNT_PIECETYPES);
	}
	return count;
}

constexpr int JUMPTABLE_PREFETCH_DISTANCE_GENERATION = (CM_ALIGNMENT_CACHELINE / sizeof(MG_MOVEINFO)) - 1;

void JUMPTABLE_GenerateQuietMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	const MG_MOVE countMoves = pMoveGen->CountMoves[movingPlayer];
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[movingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[movingPlayer][piece].TableIndex[TABLEINDEX_QUIET];
	const MG_JUMPTABLE& table = pMoveGen->JumpTable[tableIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		const MG_MOVE baseMove = table.MovesBaseFrom[fromSquareIndex];
		const BB_BITBOARD targets = pMoveGen->JumpTargets[table.TargetIndex][fromSquareIndex];
		BB_BITBOARD destinations = targets & ~pPosition->OccupancyTotal;
		BB_SQUAREINDEX toSquareIndex;
		const MG_MOVE prefetchMove = baseMove + JUMPTABLE_PREFETCH_DISTANCE_GENERATION;
		const std::uintptr_t prefetchAddrInt = (prefetchMove < countMoves) * (uintptr_t)&pMoveGen->MoveTable[movingPlayer][prefetchMove];
		CM_PREFETCH((void*)prefetchAddrInt);
		while (SQUARE_Next(destinations, toSquareIndex))
		{
			const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
			const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, targets);
			const MG_MOVE move = baseMove + optionIndex;
			MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
		}
	}
}

void JUMPTABLE_GenerateCaptureMoves(const MG_MOVEGEN* pMoveGen, MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	const MG_PLAYER movingPlayer = pPosition->Header.MovingPlayer;
	const MG_MOVE countMoves = pMoveGen->CountMoves[movingPlayer];
	const MG_PLAYER passivePlayer = pPosition->Header.PassivePlayer;
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[movingPlayer][piece];
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[movingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	MG_TABLEINDEX tableIndexByPiece[COUNT_PIECETYPES - COUNT_ROYALPIECES];
#pragma unroll
	for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
	{
		tableIndexByPiece[capturedPiece - COUNT_ROYALPIECES] = pieceInfo.TableIndex[TABLEINDEX_CAPTURE(capturedPiece)];
	}
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		for (MG_PIECETYPE capturedPiece = COUNT_ROYALPIECES; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			const MG_TABLEINDEX tableIndex = tableIndexByPiece[capturedPiece - COUNT_ROYALPIECES];
			const MG_JUMPTABLE& table = pMoveGen->JumpTable[tableIndex];
			const BB_BITBOARD targets = pMoveGen->JumpTargets[table.TargetIndex][fromSquareIndex];
			const MG_MOVE baseMove = table.MovesBaseFrom[fromSquareIndex];
			BB_BITBOARD destinations = targets & pPosition->OccupancyPlayerPiece[passivePlayer][capturedPiece];
			BB_SQUAREINDEX toSquareIndex;
			const MG_MOVE prefetchMove = baseMove + JUMPTABLE_PREFETCH_DISTANCE_GENERATION;
			const std::uintptr_t prefetchAddrInt = (prefetchMove < countMoves) * (uintptr_t)&pMoveGen->MoveTable[movingPlayer][prefetchMove];
			CM_PREFETCH((void*)prefetchAddrInt);
			while (SQUARE_Next(destinations, toSquareIndex))
			{
				const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
				const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, targets);
				const MG_MOVE move = baseMove + optionIndex;
				MOVEGEN_FinalizeMove(pMoveGen, pMoveList, pPosition, move);
			}
		}
	}
}

constexpr int JUMPTABLE_PREFETCH_DISTANCE_ATTACKS = (CM_ALIGNMENT_CACHELINE / sizeof(BB_BITBOARD)) - 1;

BB_BITBOARD JUMPTABLE_GetPieceAttacks(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, const MG_PLAYER& player, BB_BITBOARD& outInterest)
{
	BB_BITBOARD attacks = BITBOARD_EMPTY;
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[player][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_PIECEINFO& pieceInfo = pMoveGen->PieceInfo[player][piece];
	const MG_TABLEINDEX tableIndex = pieceInfo.TableIndex[TABLEINDEX_CAPTURE(0)];
	const MG_JUMPTABLE& table = pMoveGen->JumpTable[tableIndex];
	const BB_BITBOARD* pJumpTargets = pMoveGen->JumpTargets[table.TargetIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		const BB_SQUAREINDEX prefetchIndex = fromSquareIndex + JUMPTABLE_PREFETCH_DISTANCE_ATTACKS;
		const std::uintptr_t prefetchAddrInt = (prefetchIndex < COUNT_SQUARES) * (uintptr_t)&pJumpTargets[prefetchIndex];
		CM_PREFETCH((void*)prefetchAddrInt);
		const BB_BITBOARD targets = pJumpTargets[fromSquareIndex];
		attacks |= targets;
	}
	outInterest = pPosition->OccupancyPlayerPiece[player][piece];
	return attacks;
}
