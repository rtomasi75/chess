#pragma once
#ifndef SE_SEARCH_H
#define SE_SEARCH_H

#include "SE_NODE.h"
#include "SE_THREAD.h"

struct SE_CONTEXT_PERFT
{
	SE_LEAFCOUNT LeafCount;
};

SE_LEAFCOUNT SEARCH_PerftRoot(MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen, const SE_DEPTH distanceToHorizon, SE_POSITIONCOUNT& nodeCount);

#endif // SE_SEARCH_H