// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-03) initial release
//

#pragma once

#include "Engine/Core/SharedReference.h"
#include "Engine/Core/FileSystemPath.h"

extern "C" {
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoDomain MonoDomain;
}

namespace highlo
{
	struct AssemblyInfo : public IsSharedReference
	{
		FileSystemPath FilePath = "";
		MonoAssembly *Assembly = nullptr;
		MonoImage *AssemblyImage = nullptr;
		MonoDomain *Domain = nullptr;
		std::vector<uint32> Classes;
		bool IsCoreAssembly = false;
	};
}

