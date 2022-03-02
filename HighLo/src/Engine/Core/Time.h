// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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

		HLAPI static double		GetElapsedTime();
		HLAPI static double		GetFrameTime();
		HLAPI static uint32		GetFPS();

		HLAPI static double		GetTimeScale();
		HLAPI static void		SetTimeScale(double scale);

		HLAPI static Timestep	GetTimestep();
		HLAPI static void		TimeUpdate();
		HLAPI static void		FrameUpdate();

		HLAPI static double		GetSystemTime();
		HLAPI static HLString	GetCurrentTimeStamp(HLDateFormat format = HLDateFormat::English_US, HLTimePrecision precision = HLTimePrecision::Seconds);

	private:

		static double s_ElapsedTime;
		static double s_TimeScale;
		static double s_TimeStep;
		static double s_InitialTime;
		static double s_TmpSecondTimer;
		static double s_FrameTime;
		static uint32 s_FrameCounter;
		static uint32 s_FramesPerSecond;

	private:

		static double s_LastTrackedTime;
	};
}
