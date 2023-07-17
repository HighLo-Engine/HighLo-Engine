// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "EditorConsolePanel.h"

#include "Engine/ImGui/imgui.h"
#include "EditorColors.h"

namespace highlo
{
	static EditorConsolePanel *s_EditorConsoleInstance = nullptr;

	EditorConsolePanel::EditorConsolePanel()
	{
		HL_ASSERT(s_EditorConsoleInstance == nullptr);
		s_EditorConsoleInstance = this;

		m_ErrorIcon = Texture2D::LoadFromFile("assets/Resources/Icons/LogErrorIcon.png");
		m_WarningIcon = Texture2D::LoadFromFile("assets/Resources/Icons/LogWarningIcon.png");
		m_InfoIcon = Texture2D::LoadFromFile("assets/Resources/Icons/LogInfoIcon.png");
		m_TraceIcon = Texture2D::LoadFromFile("assets/Resources/Icons/LogTraceIcon.png");
	}
	
	EditorConsolePanel::~EditorConsolePanel()
	{
		s_EditorConsoleInstance = nullptr;
	}
	
	void EditorConsolePanel::OnUIRender(bool *show)
	{
		if (!(*show))
			return;

		ImGui::Begin("Log", show, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		RenderMenu();
		ImGui::Separator();
		RenderConsole();
		ImGui::End();
	}
	
	void EditorConsolePanel::OnScenePlay()
	{
		if (m_ShouldClearOnPlay)
			ClearConsole();
	}

	void EditorConsolePanel::ClearConsole()
	{
		m_MessageBufferBegin = 0;
	}

	bool EditorConsolePanel::IsConsoleCleared() const
	{
		return m_MessageBufferBegin == 0;
	}

	void EditorConsolePanel::SetMessageFilter(ConsoleMessage::LogLevel level, bool enabled)
	{
		HL_ASSERT(level != ConsoleMessage::LogLevel::None, "Invalid LogLevel: None is not supported!");

		if (enabled)
		{
			m_MessageFilters |= (int16)level;
		}
		else
		{
			m_MessageFilters &= ~(int16)level;
		}
	}

	bool EditorConsolePanel::HasMessageFilter(ConsoleMessage::LogLevel level) const
	{
		return (int16)level & m_MessageFilters;
	}

	void EditorConsolePanel::RenderMenu()
	{
		ImVec4 traceButtonTint = (m_MessageFilters & (int16)ConsoleMessage::LogLevel::Trace) ? HL_EDITOR_CONSOLE_TRACE_COLOR : HL_EDITOR_WHITE_COLOR;
		ImVec4 infoButtonTint = (m_MessageFilters & (int16)ConsoleMessage::LogLevel::Info) ? HL_EDITOR_CONSOLE_INFO_COLOR : HL_EDITOR_WHITE_COLOR;
		ImVec4 warningButtonTint = (m_MessageFilters & (int16)ConsoleMessage::LogLevel::Warning) ? HL_EDITOR_CONSOLE_WARNING_COLOR : HL_EDITOR_WHITE_COLOR;
		ImVec4 errorButtonTint = (m_MessageFilters & (int16)ConsoleMessage::LogLevel::Error) ? HL_EDITOR_CONSOLE_ERROR_COLOR : HL_EDITOR_WHITE_COLOR;
		ImVec4 greyButtonTint = HL_EDITOR_GREY_COLOR;

		constexpr float buttonOffset = 39;
		constexpr float rightSideOffset = 15;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 3));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::Button("Clear"))
			ClearConsole();

		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Clear On Play:");
		ImGui::SameLine();
		ImGui::Checkbox("##ClearOnPlay", &m_ShouldClearOnPlay);

		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Collapse:");
		ImGui::SameLine();
		ImGui::Checkbox("##CollapseMessages", &m_CollapseMessages);

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 4) - rightSideOffset);
		if (UI::ImageButton(m_TraceIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), HasMessageFilter(ConsoleMessage::LogLevel::Trace) ? traceButtonTint : greyButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Trace;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 3) - rightSideOffset);
		if (UI::ImageButton(m_InfoIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), HasMessageFilter(ConsoleMessage::LogLevel::Info) ? infoButtonTint : greyButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Info;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
		if (UI::ImageButton(m_WarningIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), HasMessageFilter(ConsoleMessage::LogLevel::Warning) ? warningButtonTint : greyButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Warning;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
		if (UI::ImageButton(m_ErrorIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), HasMessageFilter(ConsoleMessage::LogLevel::Error) ? errorButtonTint : greyButtonTint))
		{
			m_MessageFilters ^= (int16)ConsoleMessage::LogLevel::Error;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
	}
	
	void EditorConsolePanel::RenderConsole()
	{
		ImGui::BeginChild("LogMessages");

		if (m_MessageBufferBegin == 0)
		{
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

				ImGui::BeginChild(i + 1, ImVec2(0, ImGui::GetFontSize() * 1.75f), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

				if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					m_SelectedMessage = &m_MessageBuffer[i];

				HLString messageText = msg.GetMessage();

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Copy"))
						ImGui::SetClipboardText(*messageText);

					ImGui::EndPopup();
				}

				switch (msg.GetLogLevel())
				{
					case ConsoleMessage::LogLevel::Error:
					case ConsoleMessage::LogLevel::Fatal:
						UI::Image(m_ErrorIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), HL_EDITOR_CONSOLE_ERROR_COLOR);
						break;

					case ConsoleMessage::LogLevel::Trace:
						UI::Image(m_TraceIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), HL_EDITOR_CONSOLE_TRACE_COLOR);
						break;

					case ConsoleMessage::LogLevel::Warning:
						UI::Image(m_WarningIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), HL_EDITOR_CONSOLE_WARNING_COLOR);
						break;

					case ConsoleMessage::LogLevel::Info:
						UI::Image(m_InfoIcon, ImVec2(24, 24), ImVec2(0, 0), ImVec2(1, 1), HL_EDITOR_CONSOLE_INFO_COLOR);
						break;
				}

				ImGui::SameLine();

				if (messageText.Length() > 200)
				{
					uint32 spacePos = messageText.FirstIndexOf(' ', 200);
					if (spacePos != HLString::NPOS)
					{
						messageText = messageText.Substr(spacePos, messageText.Length() - 1);
						messageText += " [...]";
					}
					else
					{
						messageText = messageText.Substr(0, 200);
						messageText += " [...]";
					}
				}

				switch (msg.GetLogLevel())
				{
					case ConsoleMessage::LogLevel::Error:
					case ConsoleMessage::LogLevel::Fatal:
						ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_CONSOLE_ERROR_COLOR);
						break;

					case ConsoleMessage::LogLevel::Trace:
						ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_CONSOLE_TRACE_COLOR);
						break;

					case ConsoleMessage::LogLevel::Warning:
						ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_CONSOLE_WARNING_COLOR);
						break;

					case ConsoleMessage::LogLevel::Info:
						ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_CONSOLE_INFO_COLOR);
						break;

					default:
					case ConsoleMessage::LogLevel::None:
						ImGui::PushStyleColor(ImGuiCol_Text, HL_EDITOR_WHITE_COLOR);
						break;
				}

				ImGui::TextUnformatted(*messageText);
				ImGui::PopStyleColor();

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

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

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

