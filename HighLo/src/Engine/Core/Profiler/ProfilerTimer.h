#pragma once

#include "Profiler.h"

namespace highlo
{
	namespace utils
	{
		template<size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template<size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result{};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;

				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					++matchIndex;

				if (matchIndex == K - 1)
					srcIndex += matchIndex;

				result.Data[dstIndex] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				++srcIndex;
			}

			return result;
		}
	}

	class ProfilerTimer
	{
	public:

		ProfilerTimer(const HLString &name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartPoint = std::chrono::steady_clock::now();
		}

		~ProfilerTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartPoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch();

			Profiler::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}

	private:

		HLString m_Name;
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	};

#ifdef HIGHLO_ENABLE_PROFILER

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define HL_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define HL_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define HL_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define HL_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define HL_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define HL_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define HL_FUNC_SIG __func__
#else
#define HL_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif

#define HL_PROFILE_BEGIN_SESSION(name, filepath) Profiler::Get().BeginSession(name, filepath)
#define HL_PROFILE_END_SESSION() Profiler::Get().EndSession()
#define HL_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = utils::CleanupOutputString(name, "__cdecl "); ProfilerTimer timer##line(fixedName##line.Data);

#define HL_PROFILE_SCOPE_LINE(name, line) HL_PROFILE_SCOPE_LINE2(name, line)
#define HL_PROFILE_SCOPE(name) HL_PROFILE_SCOPE_LINE(name, __LINE__)
#define HL_PROFILE_FUNCTION() HL_PROFILE_SCOPE(HL_FUNC_SIG)

#else
#define HL_PROFILE_BEGIN_SESSION(name, filepath)
#define HL_PROFILE_END_SESSION()
#define HL_PROFILE_SCOPE(name)
#define HL_PROFILE_FUNCTION()
#endif
}