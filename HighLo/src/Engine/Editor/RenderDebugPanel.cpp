#include "HighLoPch.h"
#include "RenderDebugPanel.h"

#include "Engine/Core/Time.h"
#include "Engine/ImGui/ImGui.h"

namespace highlo
{
	RenderDebugPanel::RenderDebugPanel()
	{
	}

	RenderDebugPanel::~RenderDebugPanel()
	{
	}

	void RenderDebugPanel::OnUIRender(bool *show, float frameTime, uint32 fps, float elapsedTime)
	{
		if (!(*show))
			return;

		// Hide tab bar in docking mode
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::Begin("Renderer Debugger", show, ImGuiWindowFlags_NoTitleBar);
		
		ImGui::Text("FPS: %04d", fps);
		ImGui::SameLine();
		ImGui::Text("Frame Time: %05.1fms", frameTime);
		ImGui::SameLine();
		ImGui::Text("Elapsed Time since start: %04.1fs", elapsedTime);
		ImGui::SameLine();

		ImGui::End();
	}
	
	void RenderDebugPanel::OnEvent(Event &e)
	{
	}
}

