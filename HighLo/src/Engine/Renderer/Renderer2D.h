// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-29) Refactored DrawText functions
//     - 1.1 (2021-09-26) Added DrawText functions
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>

#include "Engine/Math/Transform.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/RenderPass.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Camera/EditorCamera.h"
#include "FontManager.h"

// Windows macro
#undef DrawText

namespace highlo
{
	struct Renderer2DStats
	{
		uint32 QuadCount = 0;
		uint32 LineCount = 0;
		uint32 TextCount = 0;
		uint32 CircleCount = 0;
		uint32 DrawCalls = 0;
	};

	class Renderer2D
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void BeginScene(const Camera &camera, bool depthTest = true);
		HLAPI static void BeginScene(const EditorCamera &camera, bool depthTest = true);
		HLAPI static void EndScene();
		HLAPI static void Flush();

		HLAPI static void StartBatch();
		HLAPI static void NextBatch();

		HLAPI static void DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, int32 entityId = 0);
		HLAPI static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, int32 entityId = 0);
		HLAPI static void DrawQuad(const Transform &transform, const glm::vec4 &color, int32 entityId = 0);

		HLAPI static void DrawTexture(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, int32 entityId = 0);
		HLAPI static void DrawTexture(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, int32 entityId = 0);
		HLAPI static void DrawTexture(const Transform &transform, const Ref<Texture2D> &texture, float tilingFactor = 1.0f, const glm::vec4 &tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, int32 entityId = 0);

		HLAPI static void DrawLine(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec4 &color, int32 entityId = 0);
		HLAPI static void DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &color, int32 entityId = 0);

		HLAPI static void FillCircle(const glm::vec2 &position, float radius, float thickness, const glm::vec4 &color, int32 entityId = 0);
		HLAPI static void FillCircle(const glm::vec3 &position, float radius, float thickness, const glm::vec4 &color, int32 entityId = 0);
		HLAPI static void FillCircle(const Transform &transform, float radius, float thickness, const glm::vec4 &color, int32 entityId = 0);

		HLAPI static void DrawCircle(const Transform &transform, float radius, const glm::vec4 &color, int32 entityId = 0);

		HLAPI static void DrawText(const HLString &text, const glm::vec3 &position, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawText(const HLString &text, const Ref<Font> &font, const glm::vec3 &position, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawText(const HLString &text, const Ref<Font> &font, const Transform &transform, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f }, float lineHeightOffset = 0.0f, float kerningOffset = 0.0f);

		HLAPI static void SetTargetRenderPass(const Ref<RenderPass> &renderPass);
		HLAPI static const Ref<RenderPass> &GetTargetRenderPass();

		HLAPI static Renderer2DStats GetStatistics();
		HLAPI static void ResetStatistics();

	private:

		static void FlushQuads();
		static void FlushCircles();
		static void FlushLines();
		static void FlushText();
	};
}

