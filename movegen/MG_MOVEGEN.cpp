#include "MG_MOVEGEN.h"
#include "MG_KING.h"
#include "MG_ROOK.h"
#include "MG_KNIGHT.h"
#include "libCommon.h"
#include <iostream>

MG_MOVE MOVEGEN_CountMoves(const MG_MOVEGEN* pMoveGen)
{
	MG_MOVE count = 0;
	count += KING_CountMoves(pMoveGen);
	count += ROOK_CountMoves(pMoveGen);
	count += KNIGHT_CountMoves(pMoveGen);
	return count;
}

void MOVEGEN_Initialize(MG_MOVEGEN* pMoveGen)
{
	pMoveGen->CountSlideEntries = SLIDEMASKS_CountEntries();
	pMoveGen->SlideEntries = new MG_SLIDEENTRY[pMoveGen->CountSlideEntries];
	MG_SLIDEENTRYINDEX nextEntry = 0;
	SLIDEMASKS_Initialize(pMoveGen, nextEntry);
	ROOK_Initialize_LookUps(pMoveGen);
	KING_Initialize_Targets(pMoveGen);
	KNIGHT_Initialize_Targets(pMoveGen);
	pMoveGen->CountMoves = MOVEGEN_CountMoves(pMoveGen);
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		// Allocate tables
		pMoveGen->MoveTable[movingPlayer] = new MG_MOVEINFO[pMoveGen->CountMoves];
	}
	for (MG_PLAYER movingPlayer = 0; movingPlayer < COUNT_PLAYERS; movingPlayer++)
	{
		// nullmove
		MG_MOVE nextMove = MOVE_NULLMOVE;
		MOVE_InitializeNullMove(pMoveGen->MoveTable[movingPlayer], movingPlayer);
		nextMove++;
		// King
		KING_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_KING]);
		KING_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			KING_Initialize_CaptureMoves(movingPlayer, capturedPiece, pMoveGen, nextMove);
		}
		// Knight
		KNIGHT_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_KNIGHT]);
		KNIGHT_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			KNIGHT_Initialize_CaptureMoves(movingPlayer, capturedPiece, pMoveGen, nextMove);
		}
		// Rook
		ROOK_Initialize_PieceInfo(&pMoveGen->PieceInfo[movingPlayer][PIECETYPE_ROOK]);
		ROOK_Initialize_QuietMoves(movingPlayer, pMoveGen, nextMove);
		ROOK_Initialize_CaptureMoves(movingPlayer, pMoveGen, nextMove);
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

void MOVEGEN_GenerateMoves(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, MG_MOVELIST* pMoveList)
{
	const MG_PLAYER movingPlayer = pPosition->MovingPlayer;
	MOVELIST_Initialize(pMoveList);
	for (MG_PIECETYPE piece = 0; piece < COUNT_PIECETYPES; piece++)
	{
		switch (pMoveGen->PieceInfo[movingPlayer][piece].MoveMechanic[MOVETYPE_QUIET])
		{
		default:
			ASSERT(false);
			break;
		case MOVEMECHANIC_JUMPTABLE:
			MOVEGEN_GenerateQuietJumps(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_SLIDETABLE:
			MOVEGEN_GenerateQuietSlides(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_NONE:
			break;
		}
		switch (pMoveGen->PieceInfo[movingPlayer][piece].MoveMechanic[MOVETYPE_CAPTURE])
		{
		case MOVEMECHANIC_JUMPTABLE:
			MOVEGEN_GenerateCaptureJumps(pMoveGen, pPosition, piece, pMoveList);
			break;
		case MOVEMECHANIC_SLIDETABLE:
			MOVEGEN_GenerateCaptureSlides(pMoveGen, pPosition, piece, pMoveList);
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

void MOVEGEN_GenerateQuietJumps(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
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

void MOVEGEN_GenerateQuietSlides(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_QUIET];
	const MG_SLIDELOOKUP& table = pMoveGen->SlideLookUp[tableIndex];
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		for (MG_SLIDEMASKINDEX slideMaskIndex = 0; slideMaskIndex < table.CountMasks; slideMaskIndex++)
		{
			const MG_SLIDEMASKINDEX maskIndex = table.MaskIndex[slideMaskIndex];
			const BB_BITBOARD mask = pMoveGen->SlideMasks[maskIndex].Mask[fromSquareIndex];
			const BB_BITBOARD potentialTargets = pMoveGen->SlideMasks[maskIndex].PotentialTargets[fromSquareIndex];
			const MG_SLIDEMASKINDEX lookUpIndex = (MG_SLIDEMASKINDEX)CM_BitExtract(pPosition->OccupancyTotal, mask);
			const MG_SLIDEENTRYINDEX entry = pMoveGen->SlideMasks[maskIndex].BaseEntry[fromSquareIndex] + lookUpIndex;
			ASSERT(entry < pMoveGen->CountSlideEntries);
			const BB_BITBOARD targets = pMoveGen->SlideEntries[entry].Targets;
			BB_BITBOARD destinations = targets & ~pPosition->OccupancyTotal;
			BB_SQUAREINDEX toSquareIndex;
			while (SQUARE_Next(destinations, toSquareIndex))
			{
				const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
				const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, potentialTargets);
				const MG_MOVE move = table.MoveBase[pPosition->MovingPlayer][slideMaskIndex][fromSquareIndex] + optionIndex;
				pMoveList->Move[pMoveList->CountMoves++] = move;
			}
		}
	}
}

void MOVEGEN_GenerateCaptureJumps(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
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

void MOVEGEN_GenerateCaptureSlides(const MG_MOVEGEN* pMoveGen, const MG_POSITION* pPosition, const MG_PIECETYPE& piece, MG_MOVELIST* pMoveList)
{
	BB_BITBOARD pieces = pPosition->OccupancyPlayerPiece[pPosition->MovingPlayer][piece];
	BB_SQUAREINDEX fromSquareIndex;
	while (SQUARE_Next(pieces, fromSquareIndex))
	{
		for (MG_PIECETYPE capturedPiece = 0; capturedPiece < COUNT_PIECETYPES; capturedPiece++)
		{
			const MG_TABLEINDEX tableIndex = pMoveGen->PieceInfo[pPosition->MovingPlayer][piece].TableIndex[TABLEINDEX_CAPTURE(capturedPiece)];
			const MG_SLIDELOOKUP& table = pMoveGen->SlideLookUp[tableIndex];
			for (MG_SLIDEMASKINDEX slideMaskIndex = 0; slideMaskIndex < table.CountMasks; slideMaskIndex++)
			{
				const MG_SLIDEMASKINDEX maskIndex = table.MaskIndex[slideMaskIndex];
				const BB_BITBOARD mask = pMoveGen->SlideMasks[maskIndex].Mask[fromSquareIndex];
				const MG_SLIDEMASKINDEX lookUpIndex = (MG_SLIDEMASKINDEX)CM_BitExtract(pPosition->OccupancyTotal, mask);
				const MG_SLIDEENTRYINDEX entry = pMoveGen->SlideMasks[maskIndex].BaseEntry[fromSquareIndex] + lookUpIndex;
				ASSERT(entry < pMoveGen->CountSlideEntries);
				BB_BITBOARD destinations = pMoveGen->SlideEntries[entry].Targets & pPosition->OccupancyPlayerPiece[pPosition->PassivePlayer][capturedPiece];
				BB_SQUAREINDEX toSquareIndex;
				while (SQUARE_Next(destinations, toSquareIndex))
				{
					const BB_SQUARE toSquare = SQUARE_FromIndex(toSquareIndex);
					const MG_OPTIONINDEX optionIndex = MOVEGEN_OptionIndex(toSquare, pMoveGen->SlideMasks[maskIndex].PotentialTargets[fromSquareIndex]);
					const MG_MOVE captureOffset = pMoveGen->SlideMasks[maskIndex].CountPotentialTargetsBits[fromSquareIndex] * capturedPiece;
					const MG_MOVE move = captureOffset + table.MoveBase[pPosition->MovingPlayer][slideMaskIndex][fromSquareIndex] + optionIndex;
					pMoveList->Move[pMoveList->CountMoves++] = move;
				}
			}
		}
	}
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
			if ((pMoveGen->MoveTable[player][move].MoveString[localPos] == 0))
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
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->MovingPlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = ~MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal &= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] &= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] &= killMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal |= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] |= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] |= createMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
}

void MOVEGEN_UnmakeMove(const MG_MOVEGEN* pMoveGen, const MG_MOVE& move, const MG_MOVEDATA* pMoveData, MG_POSITION* pPosition)
{
	const MG_MOVEINFO& moveInfo = pMoveGen->MoveTable[pPosition->PassivePlayer][move];
	if (moveInfo.MovePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD moveMap = MOVEINFO_GetMoveMap(&moveInfo);
		pPosition->OccupancyTotal ^= moveMap;
		pPosition->OccupancyPlayer[moveInfo.MovePlayer] ^= moveMap;
		pPosition->OccupancyPlayerPiece[moveInfo.MovePlayer][moveInfo.MovePiece] ^= moveMap;
	}
	if (moveInfo.CreatePiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD createMap = ~MOVEINFO_GetCreateMap(&moveInfo);
		pPosition->OccupancyTotal &= createMap;
		pPosition->OccupancyPlayer[moveInfo.CreatePlayer] &= createMap;
		pPosition->OccupancyPlayerPiece[moveInfo.CreatePlayer][moveInfo.CreatePiece] &= createMap;
	}
	if (moveInfo.KillPiece != PIECETYPE_NONE)
	{
		const BB_BITBOARD killMap = MOVEINFO_GetKillMap(&moveInfo);
		pPosition->OccupancyTotal |= killMap;
		pPosition->OccupancyPlayer[moveInfo.KillPlayer] |= killMap;
		pPosition->OccupancyPlayerPiece[moveInfo.KillPlayer][moveInfo.KillPiece] |= killMap;
	}
	MG_PLAYER tempPlayer = pPosition->PassivePlayer;
	pPosition->PassivePlayer = pPosition->MovingPlayer;
	pPosition->MovingPlayer = tempPlayer;
}
