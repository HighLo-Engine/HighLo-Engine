#pragma once

#include "RenderingAPI.h"
#include "CoreRenderer.h"
#include "ImGuiRenderer.h"
#include "Texture.h"
#include "Environment.h"

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

		HLAPI static void Init(Window *window);
		HLAPI static void Shutdown();

		HLAPI static Ref<Texture3D> GetBlackCubeTexture();
		HLAPI static Ref<Environment> GetEmptyEnvironment();
		HLAPI static RendererConfig &GetConfig();

		HLAPI static Ref<Environment> CreateEnvironment(const HLString &path);

	private:
		static UniqueRef<RenderingAPI> s_RenderingAPI;

	private:
		friend class CoreRenderer;
	};
}
