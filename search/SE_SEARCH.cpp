#include "SE_SEARCH.h"


SE_LEAFCOUNT SEARCH_Perft(SE_NODE* pNode, const MG_MOVEGEN* pMoveGen)
{
	if (pNode->DistanceToHorizon <= 0)
		return 1;
	SE_POSITIONCOUNT nodes = UINT64_C(0);
	MG_MOVELIST moveList;
	MOVELIST_Initialize(&moveList);
	MOVEGEN_GenerateMoves(pMoveGen, &pNode->Position, &moveList);
	for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
	{
		const MG_MOVE move = moveList.Move[moveIdx];
		MG_MOVEDATA moveData;
		MOVEGEN_MakeMove(pMoveGen, move, &moveData, &pNode->Position);
		pNode->DistanceToHorizon--;
#ifdef MOVEGEN_LEGAL
		pNode->NodeCount++;
		if (pNode->DistanceToHorizon == 0)
		{
			nodes++;
		}
		else
		{
			nodes += SEARCH_Perft(pNode, pMoveGen);
		}
#else
		if (POSITION_IsLegal(&pNode->Position))
		{
			pNode->NodeCount++;
			if (pNode->DistanceToHorizon == 0)
			{
				nodes++;
			}
			else
			{
				nodes += MOVEGEN_Perft(pNode, pMoveGen);
			}
		}
#endif
		pNode->DistanceToHorizon++;
		MOVEGEN_UnmakeMove(pMoveGen, move, &moveData, &pNode->Position);
	}
	return nodes;
}

SE_LEAFCOUNT SEARCH_PerftRoot(MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen, const SE_DEPTH distanceToHorizon, SE_POSITIONCOUNT& nodeCount)
{
	ASSERT(distanceToHorizon < SEARCH_MAX_DEPTH);
	SE_NODE node;
	NODE_Initialize(&node, pPosition, distanceToHorizon);
	const SE_LEAFCOUNT leafCount = SEARCH_Perft(&node, pMoveGen);
	nodeCount = node.NodeCount;
	return leafCount;
}