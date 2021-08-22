#pragma once

#include <glm/glm.hpp>

#include "Engine/Math/Transform.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Texture.h"
#include "RenderPass.h"

namespace highlo
{
	class Renderer2D
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void SetLineWidth(float width);

		HLAPI static void BeginScene(const glm::mat4 &proj, bool depthTest = true);
		HLAPI static void EndScene();
		HLAPI static void Flush();

		HLAPI static void StartBatch();
		HLAPI static void NextBatch();

		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const Transform &transform, const glm::vec4 &color);

		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawQuad(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		HLAPI static void DrawLine(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec4 &color);
		HLAPI static void DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &color);

		HLAPI static void DrawCircle(const glm::vec2 &p1, float radius, const glm::vec4 &color);
		HLAPI static void DrawCircle(const glm::vec3 &p1, float radius, const glm::vec4 &color);
	};
}