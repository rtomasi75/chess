#pragma once
#ifndef SE_NODE_H
#define SE_NODE_H

#include "libMovegen.h"
#include "SE_Types.h"

struct SE_NODE
{
	MG_POSITION Position;
	SE_DEPTH DistanceToHorizon;
	SE_POSITIONCOUNT NodeCount;
	MG_MOVEINDEX MoveIndex;
};

void NODE_Initialize(SE_NODE* pNode, const MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon);

#endif