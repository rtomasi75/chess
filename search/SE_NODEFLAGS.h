#pragma once
#ifndef SE_NODEFLAGS_H
#define SE_NODEFLAGS_H

#include <cstdint>

typedef std::uint16_t SE_NODEFLAGS;

#define NODEFLAGS_NONE			UINT16_C(0x0000)
#define NODEFLAGS_INITIALIZED	UINT16_C(0x0001)
#define NODEFLAGS_MOVELIST		UINT16_C(0x0002)

#define NODEFLAGS_IS_INITIALIZED(flags)		(((flags) & NODEFLAGS_INITIALIZED) != 0)
#define NODEFLAGS_HAS_MOVELIST(flags)		(((flags) & NODEFLAGS_MOVELIST) != 0)

#define NODEFLAGS_SET_INITIALIZED(flags)	do { flags|=NODEFLAGS_INITIALIZED; } while(false)
#define NODEFLAGS_SET_MOVELIST(flags)	do { flags|=NODEFLAGS_MOVELIST; } while(false)

#define NODEFLAGS_CLEAR(flags)	do { flags = NODEFLAGS_NONE; } while(false)

#endif // SE_NODEFLAGS_H