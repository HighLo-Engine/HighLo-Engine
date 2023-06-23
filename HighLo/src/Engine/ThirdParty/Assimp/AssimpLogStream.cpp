// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssimpLogStream.h"

namespace highlo
{
	void AssimpLogStream::Initialize()
	{
		if (Assimp::DefaultLogger::isNullLogger())
		{
			Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
			Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream, Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Warn | Assimp::Logger::Err);
		}
	}

	void AssimpLogStream::write(const char *message)
	{
		std::string msg(message);
		if (!msg.empty() && msg[msg.length() - 1] == '\n')
		{
			msg.erase(msg.length() - 1);
		}

		if (strncmp(message, "Debug", 5) == 0)
		{
			HL_CORE_TRACE(msg);
		}
		else if (strncmp(message, "Info", 4) == 0)
		{
			HL_CORE_INFO(msg);
		}
		else if (strncmp(message, "Warn", 4) == 0)
		{
			HL_CORE_WARN(msg);
		}
		else
		{
			HL_CORE_ERROR(msg);
		}
	}
}

