#include "HighLoPch.h"
#include "Log.h"

namespace highlo
{
	Ref<spdlog::logger> Logger::s_EngineLogger;
	Ref<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("Engine");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	Ref<spdlog::logger> &Logger::GetCoreLogger()
	{
		return s_EngineLogger;
	}

	Ref<spdlog::logger> &Logger::GetClientLogger()
	{
		return s_ClientLogger;
	}
}
