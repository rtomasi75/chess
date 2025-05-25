#pragma once
#ifndef SE_NODE_H
#define SE_NODE_H

#include "libMovegen.h"
#include "SE_Types.h"

struct SE_NODE
{
	SE_DEPTH DistanceToHorizon;
	SE_POSITIONCOUNT NodeCount;
	MG_MOVEINDEX MoveIndex;
	MG_MOVELIST MoveList;
	MG_MOVEDATA MoveData;
	SE_NODEFLAGS Flags;
	SE_NODESTATE State;
};

void NODE_Initialize(SE_NODE* pNode);

void NODE_GenerateMoves(SE_NODE* pNode, MG_POSITION* pPosition, const MG_MOVEGEN* pMoveGen);

#endif