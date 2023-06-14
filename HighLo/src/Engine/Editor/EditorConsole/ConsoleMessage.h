// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"

// undefine Windows macro
#undef GetMessage

namespace highlo
{
	class ConsoleMessage
	{
	public:

		enum class LogLevel
		{
			None = -1,
			Info = HL_BIT(0),
			Trace = HL_BIT(1),
			Warning = HL_BIT(2),
			Error = HL_BIT(3),
			Fatal = HL_BIT(4),
		};

		HLAPI ConsoleMessage();
		HLAPI ConsoleMessage(const HLString &msg, LogLevel level);

		HLAPI uint64 GetMessageID() const { return m_MessageID; }
		HLAPI uint32 GetCount() const { return m_Count; }
		HLAPI const HLString &GetMessage() const { return m_Message; }
		HLAPI LogLevel GetLogLevel() const { return m_LogLevel; }

	private:

		uint64 m_MessageID;
		uint32 m_Count;
		LogLevel m_LogLevel;
		HLString m_Message;

		friend class EditorConsolePanel;
	};
}

