// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Engine/ImGui/ImGuiRenderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLImGuiRenderer : public ImGuiRenderer
	{
	public:

		OpenGLImGuiRenderer();
		virtual ~OpenGLImGuiRenderer();

		virtual void Init(Window *window) override;
		virtual void Shutdown() override;

		virtual void NewFrame() override;

		virtual void Render() override;
		virtual void RenderDrawData() override;
	};
}

#endif // HIGHLO_API_OPENGL

