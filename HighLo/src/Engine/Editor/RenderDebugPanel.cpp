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

	void RenderDebugPanel::OnUIRender(bool *show)
	{
		if (!(*show))
			return;

		// Hide tab bar in docking mode
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::Begin("Renderer Debugger", show, ImGuiWindowFlags_NoTitleBar);
		
		ImGui::Text("FPS: %03d", Time::GetFPS());
		ImGui::SameLine();
	//	ImGui::Text("Frame Time: %.3fms", Time::GetFrameTime());
	//	ImGui::SameLine();
		ImGui::Text("Time Scale: %.3f", Time::GetTimeScale());
		ImGui::SameLine();
		ImGui::Text("Elapsed Time since start: %.3fs", Time::GetElapsedTime());
		ImGui::SameLine();

		ImGui::End();
	}
	
	void RenderDebugPanel::OnEvent(Event &e)
	{
	}
}

