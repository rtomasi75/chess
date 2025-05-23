#pragma once
#ifndef SE_SEARCH_H
#define SE_SEARCH_H

#include "SE_NODE.h"

#define SEARCH_MAX_DEPTH 64

SE_LEAFCOUNT SEARCH_Perft(SE_NODE* pNode, const MG_MOVEGEN* pMoveGen);

SE_LEAFCOUNT SEARCH_PerftRoot(MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen, const SE_DEPTH distanceToHorizon, SE_POSITIONCOUNT& nodeCount);

#endif // SE_SEARCH_H