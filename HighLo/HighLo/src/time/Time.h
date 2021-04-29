#pragma once
#include <core/HLCore.h>
#include <chrono>

namespace highlo
{
	typedef float Timestep;

	class Time
	{
	public:
		HLAPI static float	GetElapsedTime();

		HLAPI static float	GetTimeScale();
		HLAPI static void	SetTimeScale(float scale);

		HLAPI static Timestep	GetTimestep();
		HLAPI static void		FrameUpdate();

	private:
		static float s_ElapsedTime;
		static float s_TimeScale;
		static float s_TimeStep;
		static float s_InitialTime;

	private:
		static float s_LastTrackedTime;
	};
}
