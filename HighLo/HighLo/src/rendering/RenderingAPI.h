#pragma once
#include <core/HLCore.h>
#include <math/HLMath.h>

#include "VertexArray.h"

namespace highlo
{
	class RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) = 0;
		virtual void ClearScreenBuffers() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& va) = 0;
		virtual void DrawInstanced(const Ref<VertexArray>& va, uint32_t count) = 0;
		virtual void DrawIndexedControlPointPatchList(const Ref<VertexArray>& va) = 0;

		virtual void SetWireframe(bool wf) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	};
}