#pragma once
#ifndef MG_MOVETYPE_H
#define MG_MOVETYPE_H

#include <cstdint>

#include "MG_PIECETYPE.h"

typedef std::uint8_t MG_MOVETYPE;

#define COUNT_MOVETYPES 3

#define MOVETYPE_QUIET 0
#define MOVETYPE_CAPTURE 1
#define MOVETYPE_SPECIAL 2

#endif