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
		NODE_Initialize(pThread, pNode - 1);
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
	LOCK_Aquire(&pThread->LockNodeCount);
	pThread->NodeCount++;
	LOCK_Release(&pThread->LockNodeCount);
	return NODESTATE_PROCESS;
#else
	if (POSITION_IsLegal(&pThread->SharedPosition))
	{
		LOCK_Aquire(&pThread->LockNodeCount);
		pThread->NodeCount++;
		LOCK_Release(&pThread->LockNodeCount);
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
		LOCK_Aquire(&pSearchContext->Lock);
		pSearchContext->LeafCount++;
		LOCK_Release(&pSearchContext->Lock);
	}
}

static bool SEARCH_Iterate(SE_THREAD* pThread, SE_NODE* pNode)
{
	for (; pNode->MoveIndex < pNode->MoveList.CountMoves; pNode->MoveIndex++)
	{
		if (!FORKMASK_CheckIfMasked(&pNode->ForkMask, pNode->MoveIndex))
			break;
	}
	if (pNode->MoveIndex >= pNode->MoveList.CountMoves)
	{
		return false;
	}
	else
	{
		return true;
	}
}

static void SEARCH_Finish(SE_THREAD* pThread)
{
	pThread->DistanceToHorizon++;
}

static void SEARCH_NotifyForkComplete(SE_EXECUTIONTOKEN token, SE_SEARCHCONTEXTSTORAGE* pSearchContext, SE_THREAD* pChildThread)
{
	SE_NODE* pNode = (SE_NODE*)token;
	DISPATCH_TRACE("SEARCH: Thread %u reported completion of a fork and now joins with its parent thread %u.", pChildThread->ThreadId, pNode->pThread->ThreadId);
	const SE_FORKCOUNT previousCount = pNode->CountLiveForks.fetch_sub(1, std::memory_order_seq_cst);
	ASSERT(previousCount != 0);
	if (pNode->pThread->HostContext.Callbacks.OnAggregateSearchContext)
		pNode->pThread->HostContext.Callbacks.OnAggregateSearchContext(pNode, pSearchContext);
	LOCK_Aquire(&pNode->pThread->LockNodeCount);
	pNode->pThread->NodeCount += pChildThread->NodeCount;
	LOCK_Release(&pNode->pThread->LockNodeCount);
}

static void SEARCH_OnSuccessfulFork(SE_NODE* pNode, const SE_FORKMASK* pForkMask)
{
	pNode->CountLiveForks.fetch_add(1, std::memory_order_seq_cst);
	memcpy(&pNode->ForkMask, pForkMask, sizeof(SE_FORKMASK));
}

static void SEARCH_ForkPerft(SE_THREAD* pThread, SE_NODE* pNode)
{
	const MG_MOVEINDEX halfCount = pNode->MoveList.CountMoves / 2;
	const MG_MOVEINDEX forkCount = halfCount > SEARCH_FORK_MAX_MOVES ? SEARCH_FORK_MAX_MOVES : halfCount;
	SE_FORK fork;
	SE_FORKMASK forkMaskRoot;
	FORKMASK_Initialize(&forkMaskRoot);
	SE_CALLBACKS callbacks;
	CALLBACKS_Initialize(&callbacks, SEARCH_NotifyForkComplete);
	callbacks.OnAggregateSearchContext = SEARCH_AggregatePerftContext;
	SE_HOSTCONTEXT hostContext;
	SE_CONTEXT_PERFT searchContext;
	searchContext.LeafCount = 0;
	LOCK_Initialize(&searchContext.Lock);
	HOSTCONTEXT_Initialize(&hostContext, &callbacks, &searchContext, pNode);
	FORK_Initialize(&fork, &pThread->SharedPosition, pThread->DistanceToHorizon, pThread->StateMachine, pThread->ThreadId, &hostContext);
	for (MG_MOVEINDEX forkMoveIndex = 0; forkMoveIndex < forkCount; forkMoveIndex++)
	{
		FORKMASK_MaskMove(&forkMaskRoot, forkMoveIndex);
		FORK_AddMove(&fork, pNode->MoveList.Move[forkMoveIndex]);
	}
	if (DISPATCHER_TryFork(pThread->pDispatcher, &pThread->SharedPosition, &fork, pThread->DistanceToHorizon, pThread->StateMachine, pThread->ThreadId, &forkMaskRoot, SEARCH_OnSuccessfulFork, pNode))
	{
		DISPATCH_TRACE("SEARCH: Thread %u scheduled a fork with token=0x%016llX containing %u moves.\n", pThread->ThreadId, (std::uint64_t)pNode, forkCount);
	}
}

static void SEARCH_Join(SE_THREAD* pThread, SE_NODE* pNode)
{
	while (pNode->CountLiveForks.load(std::memory_order_seq_cst) > 0)
	{
		std::this_thread::yield();
	}
}

void SEARCH_AggregatePerftContext(SE_NODE* pParentNode, SE_SEARCHCONTEXTSTORAGE* pStorage)
{
	SE_CONTEXT_PERFT* pChild = (SE_CONTEXT_PERFT*)pStorage;
	SE_CONTEXT_PERFT* pParent = (SE_CONTEXT_PERFT*)&pParentNode->pThread->HostContext.SearchContext;
	LOCK_Aquire(&pParent->Lock);
	pParent->LeafCount += pChild->LeafCount;
	LOCK_Release(&pParent->Lock);
}

static void SEARCH_Perft_FSM_Parallel(SE_THREAD* pThread, const MG_MOVEGEN* pMoveGen)
{
	ASSERT(CONTROLFLAGS_IS_INITIALIZED(pThread->ControlFlags));
	ASSERT(pThread->DistanceToHorizon <= pThread->RootDistanceToHorizon);
	SE_CONTEXT_PERFT* pSearchContext = (SE_CONTEXT_PERFT*)&pThread->HostContext.SearchContext;
	while (pThread->DistanceToHorizon <= pThread->RootDistanceToHorizon)
	{
		SE_NODE* pNode = &pThread->Stack[pThread->DistanceToHorizon];
		ASSERT(NODEFLAGS_IS_INITIALIZED(pNode->Flags));
#ifdef SEARCH_TRACE_FSM
		char buffer[16];
		std::memset(buffer, 0, 16);
		int strPos = 0;
		const bool bStatus = NODESTATE_ToString(buffer, 16, strPos, pNode->State);
		ASSERT(bStatus);
		FSM_TRACE("FSM (PerftParallel): Thread %u | DTH %d | NodeState = %s", pThread->ThreadId, pThread->DistanceToHorizon, buffer);
#endif
		ASSERT(pNode->pThread != nullptr);
		switch (pNode->State)
		{
		default:
			ASSERT(false);
			break;
		case NODESTATE_FORK:
			SEARCH_ForkPerft(pThread, pNode);
			pNode->State = NODESTATE_ITERATE;
			break;
		case NODESTATE_EXPLORE:
			SEARCH_Explore(pThread, pNode);
			pNode->State = NODESTATE_RESUME;
			break;
		case NODESTATE_GENERATE:
			SEARCH_GenerateMoves(pNode, &pThread->SharedPosition, pMoveGen);
			pNode->State = NODESTATE_FORK;
			break;
		case NODESTATE_ITERATE:
			if (SEARCH_Iterate(pThread, pNode))
			{
				pNode->State = NODESTATE_APPLY;
			}
			else
			{
				pNode->State = NODESTATE_JOIN;
			}
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
		case NODESTATE_JOIN:
			SEARCH_Join(pThread, pNode);
			pNode->State = NODESTATE_FINISH;
			break;
		case NODESTATE_FINISH:
			SEARCH_Finish(pThread);
#ifdef _DEBUG
			pNode->State = NODESTATE_INVALID;
#endif
			break;
		}
	}
}

static void SEARCH_Perft_FSM_Sequential(SE_THREAD* pThread, const MG_MOVEGEN* pMoveGen)
{
	ASSERT(CONTROLFLAGS_IS_INITIALIZED(pThread->ControlFlags));
	SE_CONTEXT_PERFT* pSearchContext = (SE_CONTEXT_PERFT*)&pThread->HostContext.SearchContext;
	while (pThread->DistanceToHorizon <= pThread->RootDistanceToHorizon)
	{
		SE_NODE* pNode = &pThread->Stack[pThread->DistanceToHorizon];
		ASSERT(NODEFLAGS_IS_INITIALIZED(pNode->Flags));
#ifdef SEARCH_TRACE_FSM
		char buffer[16];
		std::memset(buffer, 0, 16);
		int strPos = 0;
		const bool bStatus = NODESTATE_ToString(buffer, 16, strPos, pNode->State);
		ASSERT(bStatus);
		FSM_TRACE("FSM (PerftSequential): Thread %u | DTH %d | NodeState = %s", pThread->ThreadId, pThread->DistanceToHorizon, buffer);
#endif
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
			if (SEARCH_Iterate(pThread, pNode))
			{
				pNode->State = NODESTATE_APPLY;
			}
			else
			{
				pNode->State = NODESTATE_FINISH;
			}
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
		case NODESTATE_FINISH:
			SEARCH_Finish(pThread);
#ifdef _DEBUG
			pNode->State = NODESTATE_INVALID;
#endif
			break;
		}
	}
}

void SEARCH_PerftRoot(SE_DISPATCHER* pDispatcher, const SE_CALLBACKS* pCallbacks, MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon, SE_HOSTCONTEXT* pHostContext)
{
	ASSERT(distanceToHorizon < SEARCH_MAX_DEPTH);
	DISPATCHER_Dispatch(pDispatcher, pPosition, distanceToHorizon - 1, &SEARCH_Perft_FSM_Parallel, pHostContext);
}