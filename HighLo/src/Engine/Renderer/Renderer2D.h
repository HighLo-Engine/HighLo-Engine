// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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
#include "Texture.h"
#include "RenderPass.h"
#include "FontManager.h"

// Windows macro
#undef DrawText

namespace highlo
{
	class Renderer2D
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

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

		HLAPI static void DrawText(const HLString &text, const glm::vec3 &position, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawText(const HLString &text, const Ref<Font> &font, const glm::vec3 &position, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f });
		HLAPI static void DrawText(const HLString &text, const Ref<Font> &font, const Transform &transform, float maxWidth, const glm::vec4 &color = { 1.0f, 1.0f, 1.0f, 1.0f }, float lineHeightOffset = 0.0f, float kerningOffset = 0.0f);


	};
}

