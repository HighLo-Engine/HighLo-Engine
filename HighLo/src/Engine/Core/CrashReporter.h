// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

// The crash reporter is part of the pre-compiled header, so it can not use macros that are also part of it.
// that's why we need to re-define the HLAPI macro here

#ifdef HIGHLO_USE_DLL

#ifdef HIGHLO_LIBRARY_EXPORT
#define HLAPI HL_DLL_EXPORT
#else
#define HLAPI HL_DLL_IMPORT
#endif

#else

#define HLAPI

#endif

namespace highlo
{
	class CrashReporter
	{
	public:

		HLAPI static void Crash();
	};
}

