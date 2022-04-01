// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Time.h"

#include <iomanip>

namespace highlo
{
	double Time::s_ElapsedTime = 0.0;
	double Time::s_TimeScale = 1.0;
	double Time::s_TimeStep = 0.0;
	
	double Time::s_InitialTime = -1.0;
	double Time::s_TmpSecondTimer = 0.0;
	double Time::s_LastTrackedTime = 0.0;
	double Time::s_FrameTime = 0.0;

	uint32 Time::s_FrameCounter = 0;
	uint32 Time::s_FramesPerSecond = 0;
	
	double Time::GetElapsedTime()
	{
		return s_ElapsedTime;
	}

	double Time::GetFrameTime()
	{
		return s_FrameTime;
	}

	uint32 Time::GetFPS()
	{
		return s_FramesPerSecond;
	}

	double Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(double scale)
	{
		s_TimeScale = scale;
	}

	Timestep Time::GetTimestep()
	{
		return (float)s_TimeStep;
	}

	void Time::TimeUpdate()
	{
		if (s_InitialTime == -1)
		{
			s_InitialTime = GetSystemTime();
		}

		double currentTime = GetSystemTime();

		s_ElapsedTime = (currentTime - s_InitialTime) / 1000000.0; // converting to seconds

		s_TimeStep = (s_ElapsedTime - s_LastTrackedTime) * 1000.0f; // converting to miliseconds
		s_LastTrackedTime = s_ElapsedTime;
		s_FrameTime = s_ElapsedTime;
	}

	void Time::FrameUpdate()
	{
		++s_FrameCounter;

		if (s_ElapsedTime - s_TmpSecondTimer > 1.0f)
		{
			s_FramesPerSecond = s_FrameCounter;
			s_FrameCounter = 0;
			s_TmpSecondTimer += 1.0f;
		}
	}

	double Time::GetSystemTime()
	{
		auto now = std::chrono::system_clock::now().time_since_epoch();
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(now).count();
	}

	HLString Time::GetCurrentTimeStamp(HLDateFormat format, HLTimePrecision precision)
	{
		std::time_t t = std::time(0);
		std::tm now;
		localtime_s(&now, &t);
		HLString result = "None";

		switch (format)
		{
			case HLDateFormat::English_US:
			{
				switch (precision)
				{
					case HLTimePrecision::Milliseconds:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M:%S"));
						break;
					}

					case HLTimePrecision::Seconds:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M"));
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M:%S"));
						break;
					}
				}
				break;
			}

			case HLDateFormat::English_GB:
			{
				switch (precision)
				{
					case HLTimePrecision::Milliseconds:
					{
						result = HLString::ToString(std::put_time(&now, "%d/%m/%Y - %H:%M:%S"));
						break;
					}

					case HLTimePrecision::Seconds:
					{
						result = HLString::ToString(std::put_time(&now, "%d/%m/%Y - %H:%M"));
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						result = HLString::ToString(std::put_time(&now, "%d/%m/%Y - %H:%M:%S"));
						break;
					}
				}
				break;
			}

			case HLDateFormat::German:
			{
				switch (precision)
				{
					case HLTimePrecision::Milliseconds:
					{
						result = HLString::ToString(std::put_time(&now, "%d.%m.%Y - %H:%M:%S"));
						break;
					}

					case HLTimePrecision::Seconds:
					{
						result = HLString::ToString(std::put_time(&now, "%d.%m.%Y - %H:%M"));
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						result = HLString::ToString(std::put_time(&now, "%d.%m.%Y - %H:%M:%S"));
						break;
					}
				}
				break;
			}

			default:
			{
				switch (precision)
				{
					case HLTimePrecision::Milliseconds:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M:%S"));
						break;
					}

					case HLTimePrecision::Seconds:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M"));
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						result = HLString::ToString(std::put_time(&now, "%m/%d/%Y - %H:%M:%S"));
						break;
					}
				}
				break;
			}
		}

		return result;
	}
}

