#pragma once

#include "Engine/Renderer/RenderingAPI.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLRenderingAPI : public RenderingAPI
	{
	public:
		virtual void ClearScreenColor(const glm::vec4& color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(Ref<VertexArray>& va, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawInstanced(Ref<VertexArray>& va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray>& va, PrimitiveType type = PrimitiveType::Patch) override;

		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;

		virtual Ref<Environment> CreateEnvironment(const HLString &path) override;
	};
}

#endif // HIGHLO_API_OPENGL
