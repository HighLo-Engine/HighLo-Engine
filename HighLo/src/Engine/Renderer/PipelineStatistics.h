// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "Engine/Core/Defines/BaseTypes.h"

namespace highlo
{
	struct PipelineStatistics
	{
		uint64 InputAssemblyVertices = 0;
		uint64 InputAssemblyPrimitives = 0;
		uint64 VertexShaderInvocations = 0;
		uint64 ClippingInvocations = 0;
		uint64 ClippingPrimitives = 0;
		uint64 FragmentShaderInvocations = 0;
		uint64 ComputeShaderInvocations = 0;
	};
}

