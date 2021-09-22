// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ConsoleMessage.h"

namespace highlo
{
	ConsoleMessage::ConsoleMessage()
		: m_MessageID(0), m_Message(""), m_Count(0), m_LogLevel(LogLevel::None)
	{
	}

	ConsoleMessage::ConsoleMessage(const HLString &msg, LogLevel level)
		: m_MessageID(msg.Hash()), m_Message(msg), m_Count(1), m_LogLevel(level)
	{
	}
}

