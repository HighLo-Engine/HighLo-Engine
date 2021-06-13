#include "HighLoPch.h"
#include "HLTime.h"

#include <iomanip>

namespace highlo
{
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
			s_InitialTime = (float)GetSystemTime();
		}

		double current_time = GetSystemTime();

		s_ElapsedTime = (float)((current_time - (double)s_InitialTime) / 1000000.0); // converting to seconds

		s_TimeStep = (s_ElapsedTime - s_LastTrackedTime) * 1000.0f; // converting to miliseconds
		s_LastTrackedTime = s_ElapsedTime;
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
