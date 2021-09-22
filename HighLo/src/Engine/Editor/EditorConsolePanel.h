// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-22) initial release
//

#pragma once

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

	private:

		void RenderMenu();
		void RenderConsole();
		static void PushMessage(const ConsoleMessage &message);

		bool m_ShouldClearOnPlay = false;
		bool m_CollapseMessages = false;
		bool m_NewMessageAdded = false;

		static constexpr uint32_t s_MessageBufferCapacity = 500;

		std::array<ConsoleMessage, s_MessageBufferCapacity> m_MessageBuffer;
		uint32_t m_MessageBufferBegin = 0;
		int32_t m_MessageFilters = (int16_t) ConsoleMessage::LogLevel::Info | (int16_t) ConsoleMessage::LogLevel::Warning | (int16_t) ConsoleMessage::LogLevel::Error;

		ConsoleMessage *m_SelectedMessage = nullptr;
		bool m_DisplayMessageInspector = false;
		bool m_IsMessageInspectorHovered = false;

		friend class EditorConsoleSink;
	};
}

