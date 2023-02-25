// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include <spdlog/sinks/base_sink.h>

#include "ConsoleMessage.h"

namespace highlo
{
	class EditorConsoleSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:

		HLAPI explicit EditorConsoleSink(uint32 bufferCapacity);
		HLAPI virtual ~EditorConsoleSink() = default;

		HLAPI EditorConsoleSink(const EditorConsoleSink&) = default;
		HLAPI EditorConsoleSink &operator=(const EditorConsoleSink&) = default;

	protected:

		HLAPI void sink_it_(const spdlog::details::log_msg &msg) override;
		HLAPI void flush_() override;

	private:

		uint32 m_MessageBufferCapacity;
		uint32 m_MessageCount = 0;
		std::vector<ConsoleMessage> m_MessageBuffer;
	};
}

