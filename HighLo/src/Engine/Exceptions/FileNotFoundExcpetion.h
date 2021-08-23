#pragma once

#include "ExceptionBase.h"

namespace highlo
{
	class FileNotFoundException : public Exception
	{
	public:

		FileNotFoundException(const HLString &msg)
			: Exception(msg)
		{
		}
	};
}