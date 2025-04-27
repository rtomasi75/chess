#pragma once
#ifndef MG_MOVEMECHANIC_H
#define MG_MOVEMECHANIC_H

#include <cstdint>

typedef std::uint8_t MG_MOVEMECHANIC;

#define MOVEMECHANIC_NONE 0
#define MOVEMECHANIC_JUMPTABLE 1
#define MOVEMECHANIC_SLIDETABLE 2

#define COUNT_MOVEMECHANICS 3

#endif