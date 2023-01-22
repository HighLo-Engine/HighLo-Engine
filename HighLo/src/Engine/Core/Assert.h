// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/CrashReporter.h"

#ifdef HL_DEBUG
#define HL_ENABLE_ASSERTS
#endif

#ifdef HL_ENABLE_ASSERTS

// __VA_ARGS__ expansion to get past MSVC "bug"
#define HL_EXPAND_VARGS(x) x

#define HL_ASSERT_NO_MESSAGE(x) { if(!(x)) { highlo::CrashReporter::Crash(); HL_DEBUG_BREAK; } }
#define HL_ASSERT_MESSAGE(x, ...) { if(!(x)) { HL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); highlo::CrashReporter::Crash(); HL_DEBUG_BREAK; } }

#define HL_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define HL_GET_ASSERT_MACRO(...) HL_EXPAND_VARGS(HL_ASSERT_RESOLVE(__VA_ARGS__, HL_ASSERT_MESSAGE, HL_ASSERT_NO_MESSAGE))

#define HL_ASSERT(...) HL_EXPAND_VARGS( HL_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define HL_ASSERT(...)
#endif

