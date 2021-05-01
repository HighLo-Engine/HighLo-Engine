#pragma once

#include <fstream>
#include <iomanip>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	struct ProfilerSession
	{
		HLString Name;
	};

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		HLString Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	class Profiler
	{
	public:

		Profiler(const Profiler&) = delete;
		Profiler(const Profiler&&) = delete;

		void BeginSession(const HLString &name, const HLString &filePath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				EndSessionInternal();
			}

			m_OutputStream.open(*filePath);
			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new ProfilerSession({ name });
				WriteHeader();
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			EndSessionInternal();
		}

		void WriteProfile(const ProfileResult &result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Profiler &Get()
		{
			static Profiler instance;
			return instance;
		}

	private:

		Profiler()
			: m_CurrentSession(nullptr) {}

		~Profiler()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void EndSessionInternal()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:

		std::mutex m_Mutex;
		std::ofstream m_OutputStream;
		ProfilerSession *m_CurrentSession;
	};
}