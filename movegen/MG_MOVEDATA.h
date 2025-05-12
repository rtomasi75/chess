#pragma once
#ifndef MG_MOVEDATA_H
#define MG_MOVEDATA_H

#include "MG_CASTLEFLAGS.h"
#include "MG_HASH.h"

struct MG_MOVEDATA
{
	MG_HASH OldHash;
	MG_CASTLEFLAGS OldCastlingRights;
	BB_FILEINDEX OldEnPassantFile;
};

#endif