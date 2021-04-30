#pragma once

#include "engine/core/HLCore.h"
#include "engine/core/dataTypes/HLString.h"

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
		HLAPI static float	GetElapsedTime();

		HLAPI static float	GetTimeScale();
		HLAPI static void	SetTimeScale(float scale);

		HLAPI static Timestep	GetTimestep();
		HLAPI static void		FrameUpdate();

		HLAPI static double		GetSystemTime();
		HLAPI static HLString GetCurrentTimeStamp(HLDateFormat format = HLDateFormat::English_US, HLTimePrecision precision = HLTimePrecision::Seconds);

	private:
		static float s_ElapsedTime;
		static float s_TimeScale;
		static float s_TimeStep;
		static float s_InitialTime;

	private:
		static float s_LastTrackedTime;
	};
}
