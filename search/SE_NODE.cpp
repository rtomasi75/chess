#include "SE_NODE.h"
#include "SE_THREAD.h"


void NODE_Initialize(SE_THREAD* pThread, SE_NODE* pNode)
{
	ASSERT(pThread);
	ASSERT(pNode);
	NODEFLAGS_CLEAR(pNode->Flags);
	MOVELIST_Initialize(&pNode->MoveList);
	pNode->MoveIndex = 0;
	pNode->State = NODESTATE_GENERATE;
	pNode->CountLiveForks = 0;
	pNode->pThread = pThread;
	FORKMASK_Initialize(&pNode->ForkMask);
	NODEFLAGS_SET_INITIALIZED(pNode->Flags);
}

void NODE_GenerateMoves(SE_NODE* pNode, MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen)
{
	ASSERT(!NODEFLAGS_HAS_MOVELIST(pNode->Flags));
	MOVEGEN_GenerateMoves(pMoveGen, pPosition, &pNode->MoveList);
}
