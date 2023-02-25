// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLImGuiRenderer.h"

#ifdef HIGHLO_API_OPENGL

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
#endif // HIGHLO_API_GLFW

#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"
#include "Engine/ImGui/ImGui/backends/imgui_impl_opengl3.h"

namespace highlo
{
	OpenGLImGuiRenderer::OpenGLImGuiRenderer()
	{
	}

	OpenGLImGuiRenderer::~OpenGLImGuiRenderer()
	{
	}

	void OpenGLImGuiRenderer::Init(Window *window)
	{
	#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativeHandle(), true);
	#else
		ImGui_ImplWin32_Init(window->GetNativeHandle(), window->GetNativeContext());
	#endif // HIGHLO_API_GLFW

		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void OpenGLImGuiRenderer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
	}

	void OpenGLImGuiRenderer::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
	}

	void OpenGLImGuiRenderer::Render()
	{
		ImGui::Render();
	}

	void OpenGLImGuiRenderer::RenderDrawData()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

#endif // HIGHLO_API_OPENGL

