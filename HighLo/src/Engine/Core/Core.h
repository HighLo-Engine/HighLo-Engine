#pragma once

#include <memory>

#include "Engine/Core/Defines/Compiler.h"
#include "Engine/Core/Defines/BaseTypes.h"
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
#define HL_OUT_OF_MEMORY HL_CORE_FATAL("Out of memory error!\nFile: {0}\nLine: {1}", __FILE__, __LINE__)

using HLRendererID = uint32;
using Byte = unsigned char;

namespace highlo
{
	template <typename T>
	using UniqueRef = std::unique_ptr<T>;

	template <typename T>
	using WeakRef = std::weak_ptr<T>;

	template <typename T, typename... Args>
	constexpr UniqueRef<T> MakeUniqueRef(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	struct HLAllocationMetrics
	{
		uint32 TotalAllocated = 0;	/**< The total amount of bytes allocated by the program. */
		uint32 TotalFreed = 0;		/**< The total amount of bytes freed by the program. */

		HLAPI inline uint32 CurrentUsage() { return TotalAllocated - TotalFreed; }
	};
}