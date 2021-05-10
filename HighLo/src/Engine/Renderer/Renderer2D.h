#pragma once

#include <glm/glm.hpp>

#include "Engine/Math/Transform.h"
#include "Texture.h"

namespace highlo
{
	class Renderer2D
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void BeginScene(const glm::mat4 &viewProj, bool depthTest = true);
		HLAPI static void EndScene();
		HLAPI static void Flush();

		HLAPI static void DrawQuad(const glm::mat4 &transform, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::mat4 &transform, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawQuad(const Transform &transform, const glm::vec4 &color);
		HLAPI static void DrawQuad(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		HLAPI static void DrawCircle(const glm::vec2 &p0, float radius, const glm::vec4 &color, float thickness = 0.05f);
		HLAPI static void DrawCircle(const glm::vec3 &p0, float radius, const glm::vec4 &color, float thickness = 0.05f);

		HLAPI static void DrawLine(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawLine(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });

		struct Statistics
		{
			uint32 DrawCalls = 0;
			uint32 QuadCount = 0;
			uint32 LineCount = 0;
			uint32 CircleCount = 0;

			uint32 GetTotalVertexCount() { return QuadCount * 4 + LineCount * 2 + CircleCount; }
			uint32 GetTotalIndexCount() { return QuadCount * 6 + LineCount * 2; }
		};

		HLAPI static void ResetStatistics();
		HLAPI Statistics GetStatistics();

	private:

		static void FlushAndReset();
		static void FlushAndResetLines();
	};
}