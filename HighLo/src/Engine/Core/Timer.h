// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/DataTypes/String.h"

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

		HLAPI static void Sleep(uint32 seconds);

	private:

		HLString m_Name;
		HLString m_OutputString;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	};
}
