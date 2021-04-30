#pragma once

#include "engine/Rendering/RenderingAPI.h"
#include "engine/core/Log.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11RenderingAPI : public RenderingAPI
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


#endif // HIGHLO_API_DX11
