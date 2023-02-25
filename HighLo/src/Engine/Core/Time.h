// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Core/Log.h"

namespace highlo
{
	typedef float Timestep;

	enum class HLDateFormat
	{
		None = 0,
		German,
		English_US,
		English_GB
	};

	enum class HLTimePrecision
	{
		None = 0,
		Seconds,
		Milliseconds
	};

	class Time
	{
	public:

		HLAPI static double		GetTimeScale();
		HLAPI static void		SetTimeScale(double scale);

		HLAPI static double		GetSystemTime();
		HLAPI static HLString	GetCurrentTimeStamp(HLDateFormat format = HLDateFormat::English_US, HLTimePrecision precision = HLTimePrecision::Seconds);

	private:

		static double s_TimeScale;
	};

	// TODO: Move this to a better location
	float GetTime();
}
