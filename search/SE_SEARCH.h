#pragma once
#ifndef SE_SEARCH_H
#define SE_SEARCH_H

#include "libMovegen.h"
#include "SE_NODE.h"
#include "SE_THREAD.h"
#include "SE_CALLBACKS.h"
#include "SE_HOSTCONTEXT.h"

struct SE_CONTEXT_PERFT
{
	SE_LEAFCOUNT LeafCount;
};

struct SE_DISPATCHER;

void SEARCH_PerftRoot(SE_DISPATCHER* pDispatcher, const SE_CALLBACKS* pCallbacks, MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon, SE_HOSTCONTEXT* pHostContext);

#endif // SE_SEARCH_H