#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Math/HLMath.h"

#include "VertexArray.h"
#include "Environment.h"

namespace highlo
{
	class RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) = 0;
		virtual void ClearScreenBuffers() = 0;

		virtual void DrawIndexed(Ref<VertexArray>& va) = 0;
		virtual void DrawInstanced(Ref<VertexArray>& va, uint32 count) = 0;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray>& va) = 0;

		virtual void SetWireframe(bool wf) = 0;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

		virtual Ref<Environment> CreateEnvironment(const HLString &path) = 0;
	};
}