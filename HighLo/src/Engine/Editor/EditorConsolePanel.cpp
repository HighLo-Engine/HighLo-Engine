// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "EditorConsolePanel.h"

#include "Engine/ImGui/imgui.h"

namespace highlo
{
	static EditorConsolePanel *s_EditorConsoleInstance = nullptr;

	static ImVec4 s_TraceButtonOnTint = ImVec4(0.0f, 1.0f, 0.431372549f, 1.0f);
	static ImVec4 s_InfoButtonOnTint = ImVec4(0.0f, 0.431372549f, 1.0f, 1.0f);
	static ImVec4 s_WarningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
	static ImVec4 s_ErrorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
	static ImVec4 s_NoTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	EditorConsolePanel::EditorConsolePanel()
	{
		HL_ASSERT(s_EditorConsoleInstance == nullptr);
		s_EditorConsoleInstance = this;


	}
	
	EditorConsolePanel::~EditorConsolePanel()
	{
		s_EditorConsoleInstance = nullptr;
	}
	
	void EditorConsolePanel::OnUIRender(bool *show)
	{
		if (!(*show))
			return;

		ImGui::Begin("Log", show);
		RenderMenu();
		ImGui::Separator();
		RenderConsole();
		ImGui::End();
	}
	
	void EditorConsolePanel::OnScenePlay()
	{
		if (m_ShouldClearOnPlay)
			m_MessageBufferBegin = 0;
	}

	void EditorConsolePanel::RenderMenu()
	{
		ImVec4 traceButtonTint = (m_MessageFilters & (int16_t) ConsoleMessage::LogLevel::Trace) ? s_TraceButtonOnTint : s_NoTint;
		ImVec4 infoButtonTint = (m_MessageFilters & (int16_t) ConsoleMessage::LogLevel::Info) ? s_InfoButtonOnTint : s_NoTint;
		ImVec4 warningButtonTint = (m_MessageFilters & (int16_t) ConsoleMessage::LogLevel::Warning) ? s_WarningButtonOnTint : s_NoTint;
		ImVec4 errorButtonTint = (m_MessageFilters & (int16_t) ConsoleMessage::LogLevel::Error) ? s_ErrorButtonOnTint : s_NoTint;

		constexpr float buttonOffset = 39;
		constexpr float rightSideOffset = 15;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 3));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::Button("Clear"))
			m_MessageBufferBegin = 0;

		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Clear On Play:");
		ImGui::SameLine();
		ImGui::Checkbox("##ClearOnPlay", &m_ShouldClearOnPlay);

		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Collapse:");
		ImGui::SameLine();
		ImGui::Checkbox("##CollapseMessages", &m_CollapseMessages);

		/*
		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 3) - rightSideOffset);
		if (UI::ImageButton(m_InfoButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), infoButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Info;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
		if (UI::ImageButton(m_WarningButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), warningButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Warning;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
		if (UI::ImageButton(m_ErrorButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), errorButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Error;
		}
		*/

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
	}
	
	void EditorConsolePanel::RenderConsole()
	{
		ImGui::BeginChild("LogMessages");

		if (m_MessageBufferBegin == 0)
		{
			m_DisplayMessageInspector = false;
			m_SelectedMessage = nullptr;
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_IsMessageInspectorHovered)
		{
			m_DisplayMessageInspector = false;
			m_SelectedMessage = nullptr;
		}

		for (uint32 i = 0; i < m_MessageBufferBegin; ++i)
		{
			const auto &msg = m_MessageBuffer[i];
			if (m_MessageFilters & (int16)msg.GetLogLevel())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 5));

				if (i % 2 == 0)
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(ImVec4(0.20f, 0.20f, 0.20f, 1.0f)));

				ImGui::BeginChild(i + 1, ImVec2(0, ImGui::GetFontSize() * 1.75F), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

				if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_SelectedMessage = &m_MessageBuffer[i];
					m_DisplayMessageInspector = true;
				}

				HLString messageText = msg.GetMessage();

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Copy"))
						ImGui::SetClipboardText(*messageText);

					ImGui::EndPopup();
				}

				/*
				if (msg.GetLogLevel() == ConsoleMessage::LogLevel::Info)
					UI::Image(m_InfoButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), s_InfoButtonOnTint);
				else if (msg.GetLogLevel() == ConsoleMessage::LogLevel::Warning)
					UI::Image(m_WarningButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), s_WarningButtonOnTint);
				else if (msg.GetLogLevel() == ConsoleMessage::LogLevel::Error)
					UI::Image(m_ErrorButtonTex, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), s_ErrorButtonOnTint);
				*/

				ImGui::SameLine();

				if (messageText.Length() > 200)
				{
					uint32 spacePos = messageText.FirstIndexOf(' ', 200);
					if (spacePos != HLString::NPOS)
					{
						messageText = messageText.Substr(spacePos, messageText.Length() - 1);
						messageText += "...";
					}
				}

				ImGui::TextUnformatted(*messageText);

				if (m_CollapseMessages && msg.GetCount() > 1)
				{
					ImGui::SameLine(ImGui::GetWindowWidth() - 30);
					ImGui::Text("%d", msg.GetCount());
				}

				ImGui::EndChild();

				if (i % 2 == 0)
					ImGui::PopStyleColor();

				ImGui::PopStyleVar();
			}
		}

		if (m_NewMessageAdded)
		{
			ImGui::SetScrollHereY(1.0f);
			m_NewMessageAdded = false;
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() && !m_DisplayMessageInspector)
			ImGui::SetScrollHereY(1.0f);

		if (m_DisplayMessageInspector && m_SelectedMessage != nullptr)
		{
			ImGui::Begin("Message Inspector");
			m_IsMessageInspectorHovered = ImGui::IsWindowHovered();

			ImGui::PushTextWrapPos();
			const auto &msg = m_SelectedMessage->GetMessage();
			ImGui::TextUnformatted(*msg);
			ImGui::PopTextWrapPos();

			ImGui::End();
		}
		else
		{
			m_IsMessageInspectorHovered = false;
		}

		ImGui::EndChild();
	}
	
	void EditorConsolePanel::PushMessage(const ConsoleMessage &message)
	{
		if (s_EditorConsoleInstance == nullptr)
			return;

		if (message.GetLogLevel() == ConsoleMessage::LogLevel::None)
			return;

		if (s_EditorConsoleInstance->m_CollapseMessages)
		{
			for (auto &other : s_EditorConsoleInstance->m_MessageBuffer)
			{
				if (message.GetMessageID() == other.GetMessageID())
				{
					++other.m_Count;
					return;
				}
			}
		}

		s_EditorConsoleInstance->m_MessageBuffer[s_EditorConsoleInstance->m_MessageBufferBegin++] = message;
		s_EditorConsoleInstance->m_NewMessageAdded = true;

		if (s_EditorConsoleInstance->m_MessageBufferBegin == s_MessageBufferCapacity)
			s_EditorConsoleInstance->m_MessageBufferBegin = 0;
	}
}

