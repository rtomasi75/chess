#pragma once
#ifndef CM_ASSERT_H
#define CM_ASSERT_H

#ifdef _DEBUG
#include <cassert>
#define ASSERT(_X_) assert(_X_)
#else
#if defined(_MSC_VER)
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wassume"
#endif
#define ASSERT(_X_) __assume(_X_)
#elif defined(__GNUC__)
#define ASSERT(_X_) do { if (!(_X_)) __builtin_unreachable(); } while (0)
#else
#define ASSERT(_X_) ((void)0)
#endif
#endif

#endif