#pragma once
#ifndef CM_ASSERT_H
#define CM_ASSERT_H

#ifdef _DEBUG
#include <cassert>
#define ASSERT(_X_) assert(_X_)
#else
#   if defined(__clang__)
#       pragma clang diagnostic ignored "-Wassume"
#   endif
#define ASSERT(_X_) __assume(_X_)
#endif

#endif