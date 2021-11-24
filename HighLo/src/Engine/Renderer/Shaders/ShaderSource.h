// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	struct ShaderSource
	{
		FileSystemPath FilePath;
		uint32 ShaderID = 0;

		HLString VertexShaderSrc;
		HLString TessellationControlShaderSrc;
		HLString TessellationEvaluationShaderSrc;
		HLString GeometryShaderSrc;
		HLString PixelShaderSrc;
		HLString ComputeShaderSrc;
	};
}

