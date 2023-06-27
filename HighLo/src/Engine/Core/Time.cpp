// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Time.h"

#include <iomanip>

#ifdef HL_PLATFORM_LINUX
#include <ctime>
#endif

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
#ifdef HL_PLATFORM_WINDOWS
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

#elif HL_PLATFORM_LINUX
		time_t rawtime;
		struct tm *now;
		time(&rawtime);
		now = localtime(&rawtime);
		HLString result = "None";

		switch (format)
		{
			case HLDateFormat::English_US:
			{
				switch (precision)
				{
					case HLTimePrecision::Milliseconds:
					{
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M:%S", now);
						break;
					}

					case HLTimePrecision::Seconds:
					{
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M", now);
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M:%S", now);
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
						strftime(&result[0], result.Length(), "%d/%m/%Y - %H:%M:%S", now);
						break;
					}

					case HLTimePrecision::Seconds:
					{
						strftime(&result[0], result.Length(), "%d/%m/%Y - %H:%M", now);
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						strftime(&result[0], result.Length(), "%d/%m/%Y - %H:%M:%S", now);
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
						strftime(&result[0], result.Length(), "%d.%m.%Y - %H:%M:%S", now);
						break;
					}

					case HLTimePrecision::Seconds:
					{
						strftime(&result[0], result.Length(), "%d.%m.%Y - %H:%M", now);
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						strftime(&result[0], result.Length(), "%d.%m.%Y - %H:%M:%S", now);
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
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M:%S", now);
						break;
					}

					case HLTimePrecision::Seconds:
					{
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M", now);
						break;
					}

					case HLTimePrecision::None:
					default:
					{
						strftime(&result[0], result.Length(), "%m/%d/%Y - %H:%M:%S", now);
						break;
					}
				}
				break;
			}
		}

		return result;
#endif
	}
}

