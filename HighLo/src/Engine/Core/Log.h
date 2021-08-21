#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Core.h"

namespace highlo
{
	class Logger
	{
	public:
		HLAPI static void Init();

		HLAPI static std::shared_ptr<spdlog::logger> &GetCoreLogger();
		HLAPI static std::shared_ptr<spdlog::logger> &GetClientLogger();

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define HL_CORE_ERROR(...)   ::highlo::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define HL_CORE_WARN(...)    ::highlo::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define HL_CORE_INFO(...)    ::highlo::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define HL_CORE_TRACE(...)   ::highlo::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define HL_CORE_FATAL(...)   ::highlo::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define HL_ERROR(...)   ::highlo::Logger::GetClientLogger()->error(__VA_ARGS__)
#define HL_WARN(...)    ::highlo::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define HL_INFO(...)    ::highlo::Logger::GetClientLogger()->info(__VA_ARGS__)
#define HL_TRACE(...)   ::highlo::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define HL_FATAL(...)   ::highlo::Logger::GetClientLogger()->critical(__VA_ARGS__)
