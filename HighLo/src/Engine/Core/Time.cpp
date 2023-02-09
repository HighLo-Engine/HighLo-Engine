// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Time.h"

#include <iomanip>

namespace highlo
{
	double Time::s_TimeScale = 1.0;
	
	double Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(double scale)
	{
		s_TimeScale = scale;
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

