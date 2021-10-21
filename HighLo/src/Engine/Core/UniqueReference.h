// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	template<typename T>
	class UniqueReference
	{
	public:

		HLAPI UniqueReference()
		{
			m_Data = nullptr;
		}

		HLAPI UniqueReference(T *ptr)
		{
			m_Data = ptr;
		}

		HLAPI UniqueReference(std::nullptr_t)
		{
			m_Data = nullptr;
		}

		HLAPI ~UniqueReference()
		{
			if (m_Data)
				delete m_Data;
		}

		// Delete copy operators
		HLAPI UniqueReference(const UniqueReference&) = delete;
		HLAPI UniqueReference &operator=(const UniqueReference&) = delete;

		template<typename T2>
		HLAPI UniqueReference(UniqueReference<T2> &&other)
		{
			m_Data = (T*)other.m_Data;
			other.m_Data = nullptr;
		}

		template<typename T2>
		HLAPI UniqueReference &operator=(UniqueReference<T2> &&other)
		{
			m_Data = (T*)other.m_Data;
			other.m_Data = nullptr;
			return *this;
		}

		HLAPI bool operator==(const UniqueReference<T> &other) const
		{
			return m_Data == other.m_Data;
		}

		HLAPI bool operator!=(const UniqueReference<T> &other) const
		{
			return !(*this == other);
		}

		HLAPI bool Equals(const UniqueReference<T> &other) const
		{
			if (!m_Data || !other.m_Data)
				return false;

			return *m_Data == *other.m_Data;
		}

		HLAPI bool IsValid() const
		{
			return m_Data != nullptr;
		}

		HLAPI void Reset(T *ptr = nullptr)
		{
			m_Data = ptr;
		}

		template<typename T2>
		HLAPI UniqueReference<T2> As() const
		{
			return UniqueReference<T2>(*this);
		}

		template<typename... Args>
		HLAPI static UniqueReference<T> Create(Args &&...args)
		{
			return UniqueReference<T>(new T(std::forward<Args>(args)...));
		}

		HLAPI operator bool() { return m_Data != nullptr; }
		HLAPI operator bool() const { return m_Data != nullptr; }

		HLAPI T *operator->() { return m_Data; }
		HLAPI const T *operator->() const { return m_Data; }

		HLAPI T &operator*() { return *m_Data; }
		HLAPI const T &operator*() const { return *m_Data; }

		HLAPI T *Get() { return m_Data; }
		HLAPI const T *Get() const { return m_Data; }

	private:

		template<class T2>
		friend class UniqueReference;
		T *m_Data;
	};

	template<typename T>
	using UniqueRef = UniqueReference<T>;
}

