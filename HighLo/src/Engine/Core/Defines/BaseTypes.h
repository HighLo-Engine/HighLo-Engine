// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-10-23) Added Threads limit
//     - 1.2 (2021-10-21) Removed std variable types and replaced them with our own
//     - 1.1 (2021-10-04) Added LONG and ULONG Definition
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Compiler.h"

using Byte = unsigned char;

// Unsigned base types
typedef unsigned char uint8;		// 8-bit  unsigned
typedef unsigned short uint16;		// 16-bit unsigned
typedef unsigned int uint32;		// 32-bit unsigned
typedef unsigned long ULONG;		// 32-Bit unsigned
typedef unsigned long long uint64;  // 64-bit unsigned

// Signed base types
typedef signed char int8;			// 8-bit  signed
typedef signed short int16;			// 16-bit signed
typedef signed int int32;			// 32-bit signed
typedef signed long LONG;			// 32-bit signed
typedef signed long long int64;		// 64-bit signed

// Pointer size
#ifdef HL_PLATFORM_64BITS
typedef uint64 uintptr;
typedef int64 intptr;
#define HL_POINTER_SIZE 8
#else
typedef uint32 uintptr;
typedef int32 intptr;
#define HL_POINTER_SIZE 4
#endif

// Limits
#define MIN_UINT8 ((uint8)0x00)
#define MIN_UINT16 ((uint16)0x0000)
#define MIN_UINT32 ((uint32)0x00000000)
#define MIN_UINT64 ((uint64)0x0000000000000000)
#define MIN_INT8 ((int8)-128)
#define MIN_INT16 ((int16)-32768)
#define MIN_INT32 -((int32)2147483648)
#define MIN_INT64 -((int64)9223372036854775808)
#define MIN_FLOAT -(3.402823466e+38f)
#define MIN_DOUBLE -(1.7976931348623158e+308)

#define MAX_UINT8 ((uint8)0xff)
#define MAX_UINT16 ((uint16)0xffff)
#define MAX_UINT32 ((uint32)0xffffffff)
#define MAX_UINT64 ((uint64)0xffffffffffffffff)
#define MAX_INT8 ((int8)127)
#define MAX_INT16 ((int16)32767)
#define MAX_INT32 ((int32)2147483647)
#define MAX_INT64 ((int64)9223372036854775807)
#define MAX_FLOAT (3.402823466e+38f)
#define MAX_DOUBLE (1.7976931348623158e+308)

#ifndef HL_PLATFORM_THREADS_LIMIT
#define HL_PLATFORM_THREADS_LIMIT 64
#endif

