#pragma once
#include "Rendering/RenderingAPI.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11RenderingAPI : public RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(const Ref<VertexArray>& va) override;
		virtual void DrawInstanced(const Ref<VertexArray>& va, uint32_t count) override;
		virtual void DrawIndexedControlPointPatchList(const Ref<VertexArray>& va) override;

		virtual void SetWireframe(bool wf) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}


#endif // HIGHLO_API_DX11
