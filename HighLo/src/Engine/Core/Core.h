// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <memory>

#include "Engine/Core/Defines/Compiler.h"
#include "Engine/Core/Defines/BaseTypes.h"
#include "Engine/Core/CrashReporter.h"
#include "Engine/Core/Assert.h"

#ifdef HIGHLO_USE_DLL

#ifdef HIGHLO_LIBRARY_EXPORT
#define HLAPI HL_DLL_EXPORT
#else
#define HLAPI HL_DLL_IMPORT
#endif

#else

#define HLAPI

#endif

#define HL_MAX(A, B) ( ((A)  > (B)) ? (A) :  (B) )
#define HL_MIN(A, B) ( ((A)  < (B)) ? (A) :  (B) )
#define HL_ABS(A)    ( ((A)  >= 0 ) ? (A) : -(A) )
#define HL_BIT(X)	 (1 << X)

#define HL_BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

#define HL_COPYABLE(className) HLAPI className(const className&) = default; HLAPI className &operator=(const className&) = default
#define HL_NON_COPYABLE(className) HLAPI className(const className&) = delete; HLAPI className &operator=(const className&) = delete

#define HL_MOVABLE(className) HLAPI className(className&&) = default; HLAPI className &operator=(className&&) = default
#define HL_NON_MOVABLE(className) HLAPI className(className&&) = delete; HLAPI className &operator=(className&&) = delete

using HLRendererID = uint32;
using Byte = unsigned char;

namespace highlo
{
	struct HLAllocationMetrics
	{
		uint64 TotalAllocated = 0;	/**< The total amount of bytes allocated by the program. */
		uint64 TotalFreed = 0;		/**< The total amount of bytes freed by the program. */

		HLAPI inline uint64 CurrentUsage() { return TotalAllocated - TotalFreed; }
	};
}

