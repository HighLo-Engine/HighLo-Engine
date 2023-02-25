// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef _MSC_VER

#if _MSC_VER < 1900
#error "Required Visual Studio 2015 or newer."
#endif

#define HL_DLL_EXPORT __declspec(dllexport)
#define HL_DLL_IMPORT __declspec(dllimport)
#define HL_INLINE __inline
#define HL_FORCE_INLINE __forceinline
#define HL_FORCE_NOINLINE __declspec(noinline)
#define HL_STDCALL __stdcall
#define HL_THREADLOCAL __declspec(thread)
#define HL_NORETURN __declspec(noreturn)
#define HL_PACK_BEGIN() __pragma(pack(push, 1))
#define HL_PACK_END() ; __pragma(pack(pop))
#define HL_ALIGN_BEGIN(_align) __declspec(align(_align))
#define HL_ALIGN_END(_align) /* NOTHING */
#define HL_OFFSET_OF(X, Y) offsetof(X, Y)
#define HL_DEPRECATED __declspec(deprecated)

#ifdef HL_DEBUG
	#define HL_DEBUG_BREAK __debugbreak()
#else
	#define HL_DEBUG_BREAK
#endif

#pragma warning(disable: 4251)

#elif defined(__clang__)

#define HL_DLL_EXPORT __attribute__ ((_visibility_ ("default")))
#define HL_DLL_IMPORT /* NOTHING */
#define HL_INLINE inline
#define HL_FORCE_INLINE inline
#define HL_FORCE_NOINLINE __attribute__ ((noinline))
#define HL_STDCALL __attribute__ ((stdcall))
#define HL_THREADLOCAL __thread
#define HL_NORETURN __attribute__ ((noreturn))
#define HL_PACK_BEGIN() /* NOTHING */
#define HL_PACK_END() __attribute__ ((__packed__))
#define HL_ALIGN_BEGIN(_align) /* NOTHING */
#define HL_ALIGN_END(_align) __attribute__ ((aligned(_align)))
#define HL_OFFSET_OF(X, Y) __builtin_offsetof(X, Y)
#define HL_DEPRECATED /* NOTHING */

#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wmacro-redefined"
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#pragma clang diagnostic ignored "-Wnull-dereference"
#pragma clang diagnostic ignored "-Winvalid-noreturn"

#ifdef HL_DEBUG
	#define HL_DEBUG_BREAK ??
#else
	#define HL_DEBUG_BREAK
#endif

#elif defined(__GNUC__)

#define HL_DLL_EXPORT __declspec(dllexport)
#define HL_DLL_IMPORT /* NOTHING */
#define HL_INLINE inline
#define HL_FORCE_INLINE inline
#define HL_FORCE_NOINLINE __attribute__((noinline))
#define HL_STDCALL __attribute__((stdcall))
#define HL_THREADLOCAL __thread
#define HL_NORETURN __attribute__((noreturn))
#define HL_PACK_BEGIN() /* NOTHING */
#define HL_PACK_END() __attribute__((__packed__))
#define HL_ALIGN_BEGIN(_align) /* NOTHING */
#define HL_ALIGN_END(_align) __attribute__((aligned(_align)))
#define HL_OFFSET_OF(X, Y) __builtin_offsetof(X, Y)
#define HL_DEPRECATED __attribute__((deprecated))

#ifdef HL_DEBUG
	#define HL_DEBUG_BREAK ??
#else
	#define HL_DEBUG_BREAK
#endif

#elif defined(__INTEL_COMPILER)

#define HL_DLL_EXPORT ??
#define HL_DLL_IMPORT ??
#define HL_INLINE inline
#define HL_FORCE_INLINE inline
#define HL_FORCE_NOINLINE /* NOTHING */
#define HL_STDCALL __stdcall
#define HL_NORETURN ??
#define HL_PACK_BEGIN() ??
#define HL_PACK_END() ??
#define HL_ALIGN_BEGIN(_align) ??
#define HL_ALIGN_END(_align) ??
#define HL_OFFSET_OF(X, Y) ??
#define HL_DEPRECATED ??

#ifdef _WIN32
#define HL_THREADLOCAL __declspec(thread)
#else
#define HL_THREADLOCAL __thread
#endif

#ifdef HL_DEBUG
	#define HL_DEBUG_BREAK ??
#else
	#define HL_DEBUG_BREAK
#endif

#else

#pragma error "Unknown Compiler"

#endif

#define HL_PACK_STRUCT(__declaration__) HL_PACK_BEGIN() __declaration__ HL_PACK_END()
