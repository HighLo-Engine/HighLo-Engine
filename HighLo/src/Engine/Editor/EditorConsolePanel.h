// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-24) Added MessageFilter, ClearConsole and IsConsoleCleared Function; Added LogIcons; fixed missing MessageFilter
//     - 1.1 (2021-09-23) Removed message inspector
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include "Engine/Graphics/Texture2D.h"
#include "EditorConsole/ConsoleMessage.h"

namespace highlo
{
	class EditorConsolePanel
	{
	public:

		HLAPI EditorConsolePanel();
		HLAPI ~EditorConsolePanel();

		HLAPI void OnUIRender(bool *show);
		HLAPI void OnScenePlay();

		HLAPI void ClearConsole();
		HLAPI bool IsConsoleCleared() const;

		HLAPI void SetMessageFilter(ConsoleMessage::LogLevel level, bool enabled = true);
		HLAPI bool HasMessageFilter(ConsoleMessage::LogLevel level) const;

	private:

		void RenderMenu();
		void RenderConsole();
		static void PushMessage(const ConsoleMessage &message);

		bool m_ShouldClearOnPlay = false;
		bool m_CollapseMessages  = false;
		bool m_NewMessageAdded   = false;

		static constexpr uint32 s_MessageBufferCapacity = 500;

		std::array<ConsoleMessage, s_MessageBufferCapacity> m_MessageBuffer;
		uint32 m_MessageBufferBegin = 0;
		int32 m_MessageFilters = (int16)ConsoleMessage::LogLevel::Info
			| (int16)ConsoleMessage::LogLevel::Warning
			| (int16)ConsoleMessage::LogLevel::Error
			| (int16)ConsoleMessage::LogLevel::Fatal
			| (int16)ConsoleMessage::LogLevel::Trace;

		ConsoleMessage *m_SelectedMessage = nullptr;

		// Editor Resources
		Ref<Texture2D> m_ErrorIcon;
		Ref<Texture2D> m_WarningIcon;
		Ref<Texture2D> m_InfoIcon;
		Ref<Texture2D> m_TraceIcon;

		friend class EditorConsoleSink;
	};
}

