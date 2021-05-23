#pragma once

#include "RenderingAPI.h"
#include "CoreRenderer.h"
#include "ImGuiRenderer.h"
#include "Texture.h"
#include "Environment.h"
#include "ShaderLibrary.h"
#include "RendererCapabilities.h"
#include "RenderPass.h"
#include "Model.h"
#include "Engine/Math/HLAABB.h"

namespace highlo
{
	class Window;

	struct RendererConfig
	{
		bool ComputeEnvironmentMaps = true;
	};

	class Renderer
	{
	public:

		HLAPI static void ClearScreenColor(const glm::vec4 &color);
		HLAPI static void ClearScreenBuffers();

		HLAPI static void SetWireframe(bool wf);
		HLAPI static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
		HLAPI static void SetBlendMode(bool bEnabled);
		HLAPI static void SetMultiSample(bool bEnabled);
		HLAPI static void SetDepthTest(bool bEnabled);

		HLAPI static void Init(Window *window);
		HLAPI static void Shutdown();

		HLAPI static void BeginFrame();
		HLAPI static void EndFrame();

		HLAPI static void BeginRenderPass(const Ref<RenderPass> &renderPass, bool clear = true);
		HLAPI static void EndRenderPass();

		HLAPI static void DrawAABB(const Ref<Model> &model, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));
		HLAPI static void DrawAABB(const AABB &aabb, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));

		HLAPI static Ref<Texture3D> GetBlackCubeTexture();
		HLAPI static Ref<Texture2D> GetWhiteTexture();
		HLAPI static Ref<Environment> GetEmptyEnvironment();
		HLAPI static RendererConfig &GetConfig();
		HLAPI static Ref<ShaderLibrary> GetShaderLibrary();
		HLAPI static RendererCapabilities &GetCapabilities();
		HLAPI static Ref<Texture2D> &GetBRDFLutTexture();
		HLAPI static HLString GetCurrentRenderingAPI();

		HLAPI static Ref<Environment> CreateEnvironment(const HLString &path);

	private:
		static UniqueRef<RenderingAPI> s_RenderingAPI;

	private:
		friend class CoreRenderer;
		friend class Renderer2D;
	};
}
