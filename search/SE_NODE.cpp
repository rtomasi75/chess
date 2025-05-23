#include "SE_NODE.h"


#include "SE_NODE.h"

void NODE_Initialize(SE_NODE* pNode, const MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon)
{
    memcpy(&pNode->Position, pPosition, sizeof(MG_POSITION));
    pNode->DistanceToHorizon = distanceToHorizon;
    pNode->MoveIndex = 0;
    pNode->NodeCount = 0;
}