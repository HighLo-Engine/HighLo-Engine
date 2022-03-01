// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AnimationTimelinePanel.h"

#include "Engine/ImGui/imgui.h"

namespace highlo
{
	AnimationTimelinePanel::AnimationTimelinePanel()
	{
	}

	AnimationTimelinePanel::~AnimationTimelinePanel()
	{
	}

	void AnimationTimelinePanel::OnUIRender(bool *show)
	{
		if (!(*show))
			return;

		// Hide tab bar in docking mode
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::Begin("Animation Timeline", show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		
		ImGui::PushItemWidth(85.0f);
if (ImGui::DragInt("##beginframe", &m_BeginFrame, 1, 0, 1000, "%d F")) // TODO: The max value should be the total length of the animation frames
		{
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::SetCursorPosX(100.0f);
		float backupRounding = ImGui::GetStyle().FrameRounding;
		ImGui::GetStyle().FrameRounding = 12.0f;
		ImGui::ProgressBar(10.0f, { 1400.0f, 0.0f });
		ImGui::GetStyle().FrameRounding = backupRounding;
		ImGui::SameLine();

		ImGui::PushItemWidth(85);
if (ImGui::DragInt("##endframe", &m_EndFrame, 1, 0, 1000, "%d F")) // TODO: The max value should be the total length of the animation frames
		{
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_FAST_BACKWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker("Go to the beginning\n\nJump to the beginning of the animation", false);
		ImGui::SameLine();

		ImGui::SetCursorPosX(1650);
		if (ImGui::Button(ICON_FA_BACKWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker("Go to the previous frame\n\nJump to the previous frame", false);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
		if (ImGui::Button(ICON_FA_PLAY, { 30.0f, 30.0f }))
		{

		}
		ImGui::PopStyleColor();
		UI::DrawHelpMarker("Forward play\n\nPlay the animation", false);
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_FORWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker("Go to the next frame\n\nJump to the next frame", false);
		ImGui::SameLine();

		ImGui::SetCursorPosX(1775);
		if (ImGui::Button(ICON_FA_FAST_FORWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker("Go to the end\n\nJump to the end of the animation", false);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
		ImGui::SetCursorPosX(1825);
		if (ImGui::Button(ICON_FA_KEY, { 30.0f, 30.0f }))
		{

		}
		ImGui::PopStyleColor();
		UI::DrawHelpMarker("Create keyframe\n\nCreate a keyframe of the position, scale and rotation of the current selected object.", false);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
		if (ImGui::Button(ICON_FA_RECORD_VINYL, { 30.0f, 30.0f }))
		{

		}
		ImGui::PopStyleColor();
		UI::DrawHelpMarker("Autokeying\n\nFully-automated keyframe recording", false);

		ImGui::End();
	}

	void AnimationTimelinePanel::OnEvent(Event &e)
	{
	}
}

