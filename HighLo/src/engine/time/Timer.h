#pragma once

#include "engine/core/HLCore.h"
#include "engine/core/Log.h"
#include "engine/core/dataTypes/HLString.h"

namespace highlo
{
	class Timer
	{
	public:

		HLAPI Timer(const HLString &name);
		HLAPI ~Timer();

		HLAPI void Start(const HLString &name = "");
		HLAPI void Stop(bool shouldPrint = true);

		HLAPI const HLString &GetName() const { return m_Name; }
		HLAPI const HLString &GetOutputString() const { return m_OutputString; }

	private:

		HLString m_Name;
		HLString m_OutputString;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	};
}