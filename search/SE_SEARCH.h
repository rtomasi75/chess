#pragma once
#ifndef SE_SEARCH_H
#define SE_SEARCH_H

#include "libMovegen.h"
#include "libCommon.h"
#include "SE_NODE.h"
#include "SE_THREAD.h"
#include "SE_CALLBACKS.h"
#include "SE_HOSTCONTEXT.h"

struct SE_CONTEXT_PERFT
{
	SE_LEAFCOUNT LeafCount;
	CM_LOCK Lock;
	std::uint8_t padding[sizeof(SE_SEARCHCONTEXTSTORAGE) - sizeof(SE_LEAFCOUNT) - sizeof(CM_LOCK)];
};

static_assert(sizeof(SE_CONTEXT_PERFT) == sizeof(SE_SEARCHCONTEXTSTORAGE), "Struct size mismatch!");

struct SE_DISPATCHER;

void SEARCH_PerftRoot(SE_DISPATCHER* pDispatcher, const SE_CALLBACKS* pCallbacks, MG_POSITION* pPosition, const SE_DEPTH distanceToHorizon, SE_HOSTCONTEXT* pHostContext);

void SEARCH_AggregatePerftContext(SE_NODE* pParentNode, SE_SEARCHCONTEXTSTORAGE* pStorage);


#ifdef SEARCH_TRACE_FSM
#define FSM_TRACE(fmt, ...) CM_TRACE(fmt, ##__VA_ARGS__)
#else
#define FSM_TRACE(fmt, ...) do { } while (0)
#endif


#endif // SE_SEARCH_H