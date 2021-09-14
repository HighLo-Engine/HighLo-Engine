// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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