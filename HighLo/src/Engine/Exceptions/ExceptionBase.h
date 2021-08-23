#pragma once

#include "Engine/Core/DataTypes/String.h"

#undef GetMessage // Windows pre-defined macro

namespace highlo
{
	class Exception
	{
	public:

		Exception(const HLString &msg)
			: m_ErrorMessage(msg)
		{
		}

		inline const HLString &GetMessage() const { return m_ErrorMessage; }

	private:

		HLString m_ErrorMessage;
	};
}