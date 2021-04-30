#include "HighLoPch.h"
#include "Logger.h"

namespace highlo
{
	Ref<spdlog::logger> Logger::s_EngineLogger = MakeRef<spdlog::logger>();
	Ref<spdlog::logger> Logger::s_ClientLogger = MakeRef<spdlog::logger>();

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("Engine");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
