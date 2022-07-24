/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/debug/log.hpp"

#ifdef JNG_DEBUG

#ifdef JNG_WINDOWS
#define __JNG_DEBUG_BREAK() __debugbreak()
#else
#include <signal.h>
#define __JNG_DEBUG_BREAK() raise(SIGTRAP)
#endif

#define JNG_CORE_ASSERT(x, message) if (!(x)) { JNG_CORE_FATAL("Assertion failed: {0}\nFile: {1}:{2}", message, __FILE__, __LINE__); __JNG_DEBUG_BREAK(); }
#define JNG_USER_ASSERT(x, message) if (!(x)) { JNG_USER_FATAL("Assertion failed: {0}\nFile: {1}:{2}", message, __FILE__, __LINE__); __JNG_DEBUG_BREAK(); }

#else

#define JNG_CORE_ASSERT(x, ...)
#define JNG_USER_ASSERT(x, ...)

#endif
