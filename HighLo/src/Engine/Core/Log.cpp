// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "Log.h"

namespace highlo
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("Engine");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger> &Logger::GetCoreLogger()
	{
		return s_EngineLogger;
	}

	std::shared_ptr<spdlog::logger> &Logger::GetClientLogger()
	{
		return s_ClientLogger;
	}
}
