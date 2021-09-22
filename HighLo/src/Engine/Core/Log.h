// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-22) Added Shutdown function and engine logger
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <glm/glm.hpp>

#include "Core.h"

namespace highlo
{
	class Logger
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static std::shared_ptr<spdlog::logger> &GetCoreLogger();
		HLAPI static std::shared_ptr<spdlog::logger> &GetClientLogger();
		HLAPI static std::shared_ptr<spdlog::logger> &GetEditorConsoleLogger();

	private:

		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_EditorLogger;
	};

	template<typename T>
	T &operator<<(T &os, const glm::vec2 &vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ")";
	}

	template<typename T>
	T &operator<<(T &os, const glm::vec3 &vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	}

	template<typename T>
	T &operator<<(T &os, const glm::vec4 &vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	}
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

#define HL_EDITOR_ERROR(...) ::highlo::Logger::GetEditorConsoleLogger()->error(__VA_ARGS__)
#define HL_EDITOR_WARN(...) ::highlo::Logger::GetEditorConsoleLogger()->warn(__VA_ARGS__)
#define HL_EDITOR_INFO(...) ::highlo::Logger::GetEditorConsoleLogger()->info(__VA_ARGS__)
#define HL_EDITOR_TRACE(...) ::highlo::Logger::GetEditorConsoleLogger()->trace(__VA_ARGS__)
#define HL_EDITOR_FATAL(...) ::highlo::Logger::GetEditorConsoleLogger()->critical(__VA_ARGS__)

