#pragma once

#include "engine/rendering/RenderingAPI.h"
#include "engine/logging/Logger.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLRenderingAPI : public RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(const Ref<VertexArray>& va) override;
		virtual void DrawInstanced(const Ref<VertexArray>& va, uint32 count) override;
		virtual void DrawIndexedControlPointPatchList(const Ref<VertexArray>& va) override;
		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
	};
}

#endif // HIGHLO_API_OPENGL
