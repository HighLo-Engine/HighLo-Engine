#pragma once

#include "Engine/Renderer/RenderingAPI.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11RenderingAPI : public RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(Ref<VertexArray>& va) override;
		virtual void DrawInstanced(Ref<VertexArray>& va, uint32 count) override;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray>& va) override;

		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;

		virtual Ref<Environment> CreateEnvironment(const HLString &path) override;
	};
}


#endif // HIGHLO_API_DX11
