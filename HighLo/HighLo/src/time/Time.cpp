#include "Time.h"

namespace highlo
{
	float time_since_epoch() {
		auto now = std::chrono::system_clock::now().time_since_epoch();
		return (float)std::chrono::duration_cast<std::chrono::microseconds>(now).count();
	}

	float Time::s_ElapsedTime = 0;
	float Time::s_TimeScale = 1;
	float Time::s_TimeStep = 0;

	float Time::s_InitialTime = -1;
	float Time::s_LastTrackedTime = 0;

	float Time::GetElapsedTime()
	{
		return s_ElapsedTime;
	}

	float Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(float scale)
	{
		s_TimeScale = scale;
	}

	Timestep Time::GetTimestep()
	{
		return s_TimeStep;
	}

	void Time::FrameUpdate()
	{
		if (s_InitialTime == -1)
		{
			s_InitialTime = time_since_epoch();
		}

		float current_time = time_since_epoch();

		s_ElapsedTime = (current_time - s_InitialTime) / 1000000.0f; // converting to seconds

		s_TimeStep = (s_ElapsedTime - s_LastTrackedTime) * 1000.0f; // converting to miliseconds
		s_LastTrackedTime = s_ElapsedTime;
	}
}
