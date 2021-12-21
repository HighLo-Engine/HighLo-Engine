// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Renderer/RenderingAPI.h"

namespace highlo
{
	class VulkanRenderingAPI : public RenderingAPI
	{
	public:

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void ClearScreenColor(const glm::vec4 &color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexed(uint32 indexCount, PrimitiveType type = PrimitiveType::Triangles, bool depthTest = true) override;
		virtual void DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Patch) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void SetBlendMode(bool bEnabled) override;
		virtual void SetMultiSample(bool bEnabled) override;
		virtual void SetDepthTest(bool bEnabled) override;
		virtual void SetLineThickness(float thickness) override;

		virtual Ref<Environment> CreateEnvironment(const HLString &path, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) override;
	};
}

#endif // HIGHLO_API_VULKAN

