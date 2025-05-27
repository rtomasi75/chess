#include "SE_SEARCH.h"
#include "SE_DISPATCHER.h"

static SE_LEAFCOUNT SEARCH_Perft_Recursive(SE_THREAD* pThread, const MG_MOVEGEN* pMoveGen)
{
	if (pThread->DistanceToHorizon <= 0)
		return 1;
	SE_POSITIONCOUNT nodes = UINT64_C(0);
	MG_MOVELIST moveList;
	MOVELIST_Initialize(&moveList);
	MOVEGEN_GenerateMoves(pMoveGen, &pThread->SharedPosition, &moveList);
	for (MG_MOVEINDEX moveIdx = 0; moveIdx < moveList.CountMoves; moveIdx++)
	{
		const MG_MOVE move = moveList.Move[moveIdx];
		MG_MOVEDATA moveData;
		MOVEGEN_MakeMove(pMoveGen, move, &moveData, &pThread->SharedPosition);
		pThread->DistanceToHorizon--;
#ifdef MOVEGEN_LEGAL
		pThread->NodeCount++;
		if (pThread->DistanceToHorizon == 0)
		{
			nodes++;
		}
		else
		{
			nodes += SEARCH_Perft_Recursive(pThread, pMoveGen);
		}
#else
		if (POSITION_IsLegal(&pThread->SharedPosition))
		{
			pThread->NodeCount++;
			if (pThread->DistanceToHorizon == 0)
			{
				nodes++;
			}
			else
			{
				nodes += SEARCH_Perft_Recursive(pThread, pMoveGen);
			}
		}
#endif
		pThread->DistanceToHorizon++;
		MOVEGEN_UnmakeMove(pMoveGen, move, &moveData, &pThread->SharedPosition);
	}
	return nodes;
}

static inline void SEARCH_GenerateMoves(SE_NODE* pNode, MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen)
{
	MOVEGEN_GenerateMoves(pMoveGen, pPosition, &pNode->MoveList);
}

static inline void SEARCH_Explore(SE_THREAD* pThread, SE_NODE* pNode)
{
	if (pThread->DistanceToHorizon > 0)
	{
		pThread->DistanceToHorizon--;
		NODE_Initialize(pNode - 1);
	}
}

static inline void SEARCH_Resume(SE_THREAD* pThread, SE_NODE* pNode, const MG_MOVEGEN* pMoveGen)
{
	const MG_MOVE move = pNode->MoveList.Move[pNode->MoveIndex];
	MOVEGEN_UnmakeMove(pMoveGen, move, &pNode->MoveData, &pThread->SharedPosition);
	pNode->MoveIndex++;
}

static inline SE_NODESTATE SEARCH_Apply(SE_THREAD* pThread, SE_NODE* pNode, const MG_MOVEGEN* pMoveGen)
{
	const MG_MOVE move = pNode->MoveList.Move[pNode->MoveIndex];
	MOVEGEN_MakeMove(pMoveGen, move, &pNode->MoveData, &pThread->SharedPosition);
#ifdef MOVEGEN_LEGAL
	pThread->NodeCount++;
	return NODESTATE_PROCESS;
#else
	if (POSITION_IsLegal(&pThread->SharedPosition))
	{
		pThread->NodeCount++;
		return NODESTATE_PROCESS;
	}
	else
	{
		return NODESTATE_RESUME;
	}
#endif
}

static inline void SEARCH_Perft_ProcessNode(SE_THREAD* pThread, SE_CONTEXT_PERFT* pSearchContext, const MG_MOVEGEN* pMoveGen)
{
	if (pThread->DistanceToHorizon == 0)
	{
		pSearchContext->LeafCount++;
	}
}

static SE_NODESTATE SEARCH_Iterate(SE_THREAD* pThread, SE_NODE* pNode)
{
	if (pNode->MoveIndex >= pNode->MoveList.CountMoves)
	{
		pThread->DistanceToHorizon++;
		return NODESTATE_INVALID;
	}
	else
	{
		return NODESTATE_APPLY;
	}
}

static void SEARCH_Perft_FSM(SE_THREAD* pThread, void* pContext, const MG_MOVEGEN* pMoveGen)
{
	ASSERT(CONTROLFLAGS_IS_INITIALIZED(pThread->ControlFlags));
	SE_CONTEXT_PERFT* pSearchContext = (SE_CONTEXT_PERFT*)pContext;
	while (pThread->DistanceToHorizon <= pThread->RootDistanceToHorizon)
	{
		SE_NODE* pNode = &pThread->Stack[pThread->DistanceToHorizon];
		ASSERT(NODEFLAGS_IS_INITIALIZED(pNode->Flags));
		switch (pNode->State)
		{
		default:
			ASSERT(false);
			break;
		case NODESTATE_EXPLORE:
			SEARCH_Explore(pThread, pNode);
			pNode->State = NODESTATE_RESUME;
			break;
		case NODESTATE_GENERATE:
			SEARCH_GenerateMoves(pNode, &pThread->SharedPosition, pMoveGen);
			pNode->State = NODESTATE_ITERATE;
			break;
		case NODESTATE_ITERATE:
			pNode->State = SEARCH_Iterate(pThread, pNode);
			break;
		case NODESTATE_APPLY:
			pNode->State = SEARCH_Apply(pThread, pNode, pMoveGen);
			break;
		case NODESTATE_PROCESS:
			SEARCH_Perft_ProcessNode(pThread, pSearchContext, pMoveGen);
			pNode->State = NODESTATE_EXPLORE;
			break;
		case NODESTATE_RESUME:
			SEARCH_Resume(pThread, pNode, pMoveGen);
			pNode->State = NODESTATE_ITERATE;
			break;
		}
	}
}

void SEARCH_PerftRoot(SE_DISPATCHER* pDispatcher, const SE_CALLBACKS* pCallbacks, MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon, SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(distanceToHorizon < SEARCH_MAX_DEPTH);
	DISPATCHER_Dispatch(pDispatcher, pPosition, distanceToHorizon - 1, &SEARCH_Perft_FSM, pHostContext);
}