#pragma once

#include "ExceptionBase.h"

namespace highlo
{
	class ShaderCompilationFailedException : public Exception
	{
	public:

		ShaderCompilationFailedException(const HLString &file, int32 line)
			: Exception(file, line, "Shader compilation failed")
		{
		}
	};
}