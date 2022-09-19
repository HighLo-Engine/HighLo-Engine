// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-19) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	template<typename T>
	class Optional
	{
	public:

		HLAPI Optional()
		{
			m_HasValue = false;
			m_Data = nullptr;
		}

		HLAPI Optional(T *ptr)
		{
			m_HasValue = true;
			m_Data = ptr;
		}

		HLAPI Optional(T &ref)
		{
			m_HasValue = true;
			m_Data = &ref;
		}

		HLAPI Optional<T> &operator=(const Optional<T> &other)
		{
			m_Data = other.m_Data;
			m_HasValue = other.m_HasValue;
			return *this;
		}

		HLAPI Optional<T> &operator=(T &value)
		{
			m_Data = &value;
			m_HasValue = true;
			return *this;
		}

		HLAPI ~Optional()
		{
		}

		HLAPI const T *operator*() const
		{
			return m_HasValue ? m_Data : nullptr;
		}

		HLAPI bool HasValue() const { return m_HasValue; }

		HLAPI T *Value()
		{
			if (m_HasValue)
				return m_Data;

			return nullptr;
		}

		HLAPI T *ValueOrDefault(const T &default)
		{
			return m_HasValue ? m_Data : &default;
		}

		HLAPI void Reset()
		{
			m_Data = nullptr;
			m_HasValue = false;
		}

	private:

		bool m_HasValue = false;
		T *m_Data = nullptr;
	};
}

