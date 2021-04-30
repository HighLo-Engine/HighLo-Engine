#pragma once

#include "engine/core/HLCore.h"
#include "engine/math/HLMath.h"

#include "VertexArray.h"

namespace highlo
{
	class RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) = 0;
		virtual void ClearScreenBuffers() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& va) = 0;
		virtual void DrawInstanced(const Ref<VertexArray>& va, uint32 count) = 0;
		virtual void DrawIndexedControlPointPatchList(const Ref<VertexArray>& va) = 0;

		virtual void SetWireframe(bool wf) = 0;

		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	};
}