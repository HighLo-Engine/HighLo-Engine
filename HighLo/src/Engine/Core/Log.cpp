// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Log.h"

#include "Engine/Editor/EditorConsole/EditorConsoleSink.h"

namespace highlo
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Logger::s_EditorLogger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		std::vector<spdlog::sink_ptr> engineSinks =
		{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine.log", true)
		};

		std::vector<spdlog::sink_ptr> appSinks =
		{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", true),
			std::make_shared<EditorConsoleSink>(1)
		};

		std::vector<spdlog::sink_ptr> editorConsoleSinks =
		{
			std::make_shared<EditorConsoleSink>(1)
		};

		engineSinks[0]->set_pattern("%^[%T] %n: %v%$");
		engineSinks[1]->set_pattern("[%T] [%l] %n: %v");
		appSinks[0]->set_pattern("%^[%T] %n: %v%$");
		appSinks[1]->set_pattern("[%T] [%l] %n: %v");
		appSinks[2]->set_pattern("%^[%T] %n: %v%$");
		editorConsoleSinks[0]->set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = std::make_shared<spdlog::logger>("Engine", engineSinks.begin(), engineSinks.end());
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Client", appSinks.begin(), appSinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);

		s_EditorLogger = std::make_shared<spdlog::logger>("Console", editorConsoleSinks.begin(), editorConsoleSinks.end());
		s_EditorLogger->set_level(spdlog::level::trace);
	}

	void Logger::Shutdown()
	{
		s_EditorLogger.reset();
		s_ClientLogger.reset();
		s_EngineLogger.reset();

		spdlog::drop_all();
	}

	std::shared_ptr<spdlog::logger> &Logger::GetCoreLogger()
	{
		return s_EngineLogger;
	}
	
	std::shared_ptr<spdlog::logger> &Logger::GetClientLogger()
	{
		return s_ClientLogger;
	}
	
	std::shared_ptr<spdlog::logger> &Logger::GetEditorConsoleLogger()
	{
		return s_EditorLogger;
	}
}

