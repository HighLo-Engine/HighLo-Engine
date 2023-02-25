// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "CrashReporter.h"

#include "Engine/Graphics/Shaders/ShaderCache.h"

namespace highlo
{
	void CrashReporter::Crash()
	{
		HL_CORE_FATAL("The Engine is about to crash! going to write recovery files and all caches into their files!");
		ShaderCache::Shutdown();
	}
}

