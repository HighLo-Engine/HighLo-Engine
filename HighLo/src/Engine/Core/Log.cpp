// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Log.h"

#include "Engine/Application/Application.h"
#include "Engine/Editor/EditorConsole/EditorConsoleSink.h"

namespace highlo
{
	static std::shared_ptr<spdlog::logger> s_EngineLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
	static std::shared_ptr<spdlog::logger> s_EditorLogger;

	void Logger::Init()
	{
		bool shouldLogToFile = false;
		if (HLApplication::HasStarted())
		{
			shouldLogToFile = !HLApplication::Get().GetApplicationSettings().NoLog;
		}

		std::vector<spdlog::sink_ptr> engineSinks;
		std::vector<spdlog::sink_ptr> appSinks;

		if (shouldLogToFile)
		{
			engineSinks =
			{
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
				std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine.log", true),
				std::make_shared<EditorConsoleSink>(1)
			};

			appSinks =
			{
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
				std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", true),
				std::make_shared<EditorConsoleSink>(1)
			};
		}
		else
		{
			engineSinks =
			{
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
				std::make_shared<EditorConsoleSink>(1)
			};

			appSinks =
			{
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
				std::make_shared<EditorConsoleSink>(1)
			};
		}

		std::vector<spdlog::sink_ptr> editorConsoleSinks =
		{
			std::make_shared<EditorConsoleSink>(1)
		};

		// Engine patterns
		engineSinks[0]->set_pattern("%^[%T] %n: %v%$");
		engineSinks[1]->set_pattern("[%T] [%l] %n: %v");
		if (shouldLogToFile)
		{
			engineSinks[2]->set_pattern("[%T] [%l] %n: %v");
		}

		// App patterns
		appSinks[0]->set_pattern("%^[%T] %n: %v%$");
		appSinks[1]->set_pattern("[%T] [%l] %n: %v");
		if (shouldLogToFile)
		{
			appSinks[2]->set_pattern("%^[%T] %n: %v%$");
		}

		// Editor Patterns
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

