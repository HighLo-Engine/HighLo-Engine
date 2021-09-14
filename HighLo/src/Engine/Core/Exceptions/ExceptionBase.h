// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

// TODO: make a better abstraction system for the platform, because the windows version of GetMessage should not be accessible here
#undef GetMessage // Windows pre-defined macro

#define HL_THROW_EXCEPTION(Exception)  throw(Exception(__FILE__, __LINE__))

namespace highlo
{
	class Exception
	{
	public:

		HLAPI Exception(const HLString &file, int32 line, const HLString &msg)
		{
			m_Message = file + ":" + HLString::ToString(line) + ": " + msg;
		}

		HLAPI ~Exception()
		{
		}

		// Default copy constructor and copy operator
		HLAPI Exception(const Exception&) = default;
		HLAPI Exception &operator=(const Exception&) = default;

		HLAPI inline const HLString &GetMessage() const { return m_Message; }

	private:

		HLString m_Message;
	};
}