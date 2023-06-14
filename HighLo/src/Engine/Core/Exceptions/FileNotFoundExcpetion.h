// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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

