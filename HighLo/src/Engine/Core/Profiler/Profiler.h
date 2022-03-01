// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <fstream>
#include <iomanip>

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct HLAPI ProfilerSession
	{
		HLString Name;
	};

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct HLAPI ProfileResult
	{
		HLString Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	class Profiler
	{
	public:

		// Disable copy and move constructors
		HL_NON_COPYABLE(Profiler);
		HL_NON_MOVABLE(Profiler);

		HLAPI void BeginSession(const HLString &name, const HLString &filePath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
				EndSessionInternal();

			m_OutputStream.open(*filePath);
			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new ProfilerSession({ name });
				WriteHeader();
			}
		}

		HLAPI void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			EndSessionInternal();
		}

		HLAPI void WriteProfile(const ProfileResult &result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << *result.Name << "\",";
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

		HLAPI static Profiler &Get()
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

