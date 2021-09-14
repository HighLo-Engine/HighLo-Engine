// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Timer.h"

namespace highlo
{
	Timer::Timer(const HLString &name)
		: m_Name(name), m_Stopped(false) {}

	Timer::~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Timer::Start(const HLString &name)
	{
		if (!m_Name.IsEmpty())
		{
			m_Name = name;
		}

		m_Stopped = false;
		m_OutputString.Clear();
		m_StartPoint = std::chrono::high_resolution_clock::now();
	}

	void Timer::Stop(bool shouldPrint)
	{
		auto endPoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endPoint).time_since_epoch().count();

		m_OutputString << m_Name << " Time: " << (end - start) << "ms";
		m_Stopped = true;

		if (shouldPrint)
		{
			HL_CORE_TRACE("{0}", *m_OutputString);
		}
	}
}
