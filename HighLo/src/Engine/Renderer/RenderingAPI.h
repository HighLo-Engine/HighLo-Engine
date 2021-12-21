// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"

#include "VertexArray.h"
#include "Environment.h"

namespace highlo
{
	enum class PrimitiveType
	{
		None = 0,
		Triangles,
		Lines,
		Patch
	};

	class RenderingAPI
	{
	public:

		HLAPI virtual void Init() = 0;
		HLAPI virtual void Shutdown() = 0;

		HLAPI virtual void ClearScreenColor(const glm::vec4 &color) = 0;
		HLAPI virtual void ClearScreenBuffers() = 0;

		HLAPI virtual void DrawIndexed(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Triangles) = 0;
		HLAPI virtual void DrawIndexed(uint32 indexCount, PrimitiveType type = PrimitiveType::Triangles, bool depthTest = true) = 0;
		HLAPI virtual void DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) = 0;
		HLAPI virtual void DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Patch) = 0;

		HLAPI virtual void BeginFrame() = 0;
		HLAPI virtual void EndFrame() = 0;

		HLAPI virtual void SetWireframe(bool wf) = 0;
		HLAPI virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		HLAPI virtual void SetBlendMode(bool bEnabled) = 0;
		HLAPI virtual void SetMultiSample(bool bEnabled) = 0;
		HLAPI virtual void SetDepthTest(bool bEnabled) = 0;
		HLAPI virtual void SetLineThickness(float thickness) = 0;

		HLAPI virtual Ref<Environment> CreateEnvironment(const HLString &path, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) = 0;
	};
}

