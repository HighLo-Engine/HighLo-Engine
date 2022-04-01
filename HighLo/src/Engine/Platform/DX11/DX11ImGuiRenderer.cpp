#include "HighLoPch.h"
#include "DX11ImGuiRenderer.h"

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
#endif // HIGHLO_API_GLFW

#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"
#include "Engine/ImGui/ImGui/backends/imgui_impl_dx11.h"

#ifdef HIGHLO_API_DX11

#include "DX11Resources.h"

namespace highlo
{
	DX11ImGuiRenderer::DX11ImGuiRenderer()
	{
	}

	DX11ImGuiRenderer::~DX11ImGuiRenderer()
	{
	}

	void DX11ImGuiRenderer::Init(Window *window)
	{
	#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_InitForOther((GLFWwindow*)window->GetNativeHandle(), true);
	#else
		ImGui_ImplWin32_Init(window->GetNativeHandle(), window->GetNativeContext());
	#endif // HIGHLO_API_GLFW

		ImGui_ImplDX11_Init(DX11Resources::s_Device.Get(), DX11Resources::s_DeviceContext.Get());
	}

	void DX11ImGuiRenderer::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}
	
	void DX11ImGuiRenderer::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();

	#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_NewFrame();
	#endif // HIGHLO_API_GLFW
	}
	
	void DX11ImGuiRenderer::Render()
	{
		ImGui::Render();
	}
	
	void DX11ImGuiRenderer::RenderDrawData()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

#endif // HIGHLO_API_DX11

