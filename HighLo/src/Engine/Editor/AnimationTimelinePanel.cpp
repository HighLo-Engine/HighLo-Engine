// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AnimationTimelinePanel.h"

#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/ImGuiAnimationTimeline.h"

#include "Engine/Application/Application.h"

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

		Translation *translation = HLApplication::Get().GetActiveTranslation();

		// Hide tab bar in docking mode
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoResize;
		ImGui::SetNextWindowClass(&windowClass);

		ImGui::Begin("Animation Timeline", show, ImGuiWindowFlags_NoTitleBar);
		
		// Animation timeline
		if (UI::BeginAnimationTimeline("##animTimeline", { 1800.0f, 0.0f }, ImGuiAnimationTimelineFlags_None | ImGuiAnimationTimelineFlags_Test))
		{


			UI::EndAnimationTimeline();
		}

		float backupRounding = ImGui::GetStyle().FrameRounding;
		ImGui::GetStyle().FrameRounding = 12.0f;
		ImGui::ProgressBar(0.0f, { 1800.0f, 0.0f });
		ImGui::GetStyle().FrameRounding = backupRounding;
		ImGui::SameLine();

		ImGui::PushItemWidth(85.0f);
		if (ImGui::DragInt("##totalFrames", &m_NumFrames, 1, 1, 1000, "%d F")) // TODO: The max value should be the total length of the animation frames
		{
		}
		ImGui::PopItemWidth();

		// Animation controls
		ImGui::NewLine();

		ImGui::PushItemWidth(85.0f);
		if (ImGui::DragInt("##beginframe", &m_BeginFrame, 1, 0, 1000, "%d F")) // TODO: The max value should be the total length of the animation frames
		{
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::SetCursorPosX(100.0f);
		backupRounding = ImGui::GetStyle().FrameRounding;
		ImGui::GetStyle().FrameRounding = 12.0f;
		ImGui::ProgressBar(1.0f, { 1400.0f, 0.0f });
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
		UI::DrawHelpMarker(translation->GetText("animation-timeline-begin-btn-tooltip"), false);
		ImGui::SameLine();

		ImGui::SetCursorPosX(1650);
		if (ImGui::Button(ICON_FA_BACKWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker(translation->GetText("animation-timeline-prev-frame-btn-tooltip"), false);
		ImGui::SameLine();

		if (m_AnimPlaying)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
			if (ImGui::Button(ICON_FA_PAUSE, { 30.0f, 30.0f }))
			{
				m_AnimPausing = true;
				m_AnimPlaying = false;
			}
			ImGui::PopStyleColor();
			UI::DrawHelpMarker(translation->GetText("animation-timeline-pause-btn-tooltip"), false);
			ImGui::SameLine();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
			if (ImGui::Button(ICON_FA_PLAY, { 30.0f, 30.0f }))
			{
				m_AnimPausing = false;
				m_AnimPlaying = true;
			}
			ImGui::PopStyleColor();
			UI::DrawHelpMarker(translation->GetText("animation-timeline-play-btn-tooltip"), false);
			ImGui::SameLine();
		}

		if (ImGui::Button(ICON_FA_FORWARD, { 30.0f, 30.0f }))
		{

		}
		UI::DrawHelpMarker(translation->GetText("animation-timeline-next-frame-btn-tooltip"), false);
		ImGui::SameLine();

		ImGui::SetCursorPosX(1775);
		if (ImGui::Button(ICON_FA_FAST_FORWARD, { 30.0f, 30.0f }))
		{
			// Skip the animation to the end of the animation
		}
		UI::DrawHelpMarker(translation->GetText("animation-timeline-end-btn-tooltip"), false);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
		ImGui::SetCursorPosX(1825);
		if (ImGui::Button(ICON_FA_KEY, { 30.0f, 30.0f }))
		{
			// Create a new keyframe
		}
		ImGui::PopStyleColor();
		UI::DrawHelpMarker(translation->GetText("animation-timeline-keyframe-btn-tooltip"), false);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
		if (ImGui::Button(ICON_FA_RECORD_VINYL, { 30.0f, 30.0f }))
		{
			// auto-keying is activated, so now every time a model has been transformed a new keyframe should be created at the current timeline position
		}
		ImGui::PopStyleColor();
		UI::DrawHelpMarker(translation->GetText("animation-timeline-autokeying-btn-tooltip"), false);

		ImGui::End();
	}

	void AnimationTimelinePanel::OnEvent(Event &e)
	{
	}
}

