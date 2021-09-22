// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "EditorConsoleSink.h"

#include "Engine/Editor/EditorConsolePanel.h"

namespace highlo
{
	namespace utils
	{
		static ConsoleMessage::LogLevel GetMessageLogLevel(spdlog::level::level_enum level)
		{
			switch (level)
			{
				case spdlog::level::trace:
					return ConsoleMessage::LogLevel::Trace;

				case spdlog::level::debug:
				case spdlog::level::info:
					return ConsoleMessage::LogLevel::Info;
				
				case spdlog::level::warn:
					return ConsoleMessage::LogLevel::Warning;

				case spdlog::level::err:
					return ConsoleMessage::LogLevel::Error;

				case spdlog::level::critical:
					return ConsoleMessage::LogLevel::Fatal;
			}

			HL_ASSERT(false, "Invalid Message Category!");
			return ConsoleMessage::LogLevel::None;
		}
	}

	EditorConsoleSink::EditorConsoleSink(uint32 bufferCapacity)
		: m_MessageBufferCapacity(bufferCapacity), m_MessageBuffer(bufferCapacity)
	{
	}
	
	void EditorConsoleSink::sink_it_(const spdlog::details::log_msg &msg)
	{
		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
		m_MessageBuffer[m_MessageCount++] = ConsoleMessage(fmt::to_string(formatted), utils::GetMessageLogLevel(msg.level));

		if (m_MessageCount == m_MessageBufferCapacity)
			flush_();
	}
	
	void EditorConsoleSink::flush_()
	{
		for (const auto &message : m_MessageBuffer)
		{
			if (message.GetLogLevel() == ConsoleMessage::LogLevel::None)
				continue;

			EditorConsolePanel::PushMessage(message);
		}

		m_MessageCount = 0;
	}
}

