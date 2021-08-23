#pragma once

#include "ExceptionBase.h"

namespace highlo
{
	class FileNotFoundException : public Exception
	{
	public:

		FileNotFoundException(const HLString &file, int32 line)
			: Exception(file, line, "File not found")
		{
		}
	};
}

