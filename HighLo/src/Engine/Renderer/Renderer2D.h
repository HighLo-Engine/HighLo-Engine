#pragma once

#include <glm/glm.hpp>

#include "Engine/Math/Transform.h"
#include "Texture.h"
#include "RenderPass.h"

namespace highlo
{
	class Renderer2D
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static Ref<RenderPass> GetRenderPass();
		HLAPI static void SetRenderPass(Ref<RenderPass> renderPass);
		HLAPI static void SetLineWidth(float width);

		HLAPI static void BeginScene(const glm::mat4 &proj, bool depthTest = true);
		HLAPI static void EndScene();
		HLAPI static void Flush();

		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
	};
}