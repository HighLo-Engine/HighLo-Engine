// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	class IsSharedReference
	{
	public:

		void IncrementReferenceCount() const
		{
			m_ReferenceCount++;
		}

		void DecrementReferenceCount() const
		{
			m_ReferenceCount--;
		}

		inline uint32 GetReferenceCount() const { return m_ReferenceCount; }

	private:

		mutable uint32 m_ReferenceCount = 0;
	};

	template<typename T>
	class SharedReference
	{
	public:

		HLAPI SharedReference()
			: m_Instance(nullptr) {}

		HLAPI SharedReference(std::nullptr_t)
			: m_Instance(nullptr) {}

		HLAPI SharedReference(T *instance)
			: m_Instance(instance)
		{
			static_assert(std::is_base_of<IsSharedReference, T>::value, "Class is not RefCounted!");
			IncRef();
		}

		template<typename T2>
		HLAPI SharedReference(const SharedReference<T2> &other)
		{
			m_Instance = (T*)other.m_Instance;
			IncRef();
		}

		template<typename T2>
		HLAPI SharedReference(SharedReference<T2> &&other)
		{
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
		}

		HLAPI ~SharedReference()
		{
			DecRef();
		}

		HLAPI SharedReference(const SharedReference<T> &other)
			: m_Instance(other.m_Instance)
		{
			IncRef();
		}

		HLAPI SharedReference &operator=(std::nullptr_t)
		{
			DecRef();
			m_Instance = nullptr;
			return *this;
		}

		HLAPI SharedReference &operator=(const SharedReference<T> &other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		HLAPI SharedReference &operator=(const SharedReference<T2> &other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		HLAPI SharedReference &operator=(SharedReference<T2> &&other)
		{
			DecRef();

			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}

		HLAPI operator bool() { return m_Instance != nullptr; }
		HLAPI operator bool() const { return m_Instance != nullptr; }

		HLAPI T *operator->() { return m_Instance; }
		HLAPI const T *operator->() const { return m_Instance; }

		HLAPI T &operator*() { return *m_Instance; }
		HLAPI const T &operator*() const { return *m_Instance; }

		HLAPI T *Get() { return m_Instance; }
		HLAPI const T *Get() const { return m_Instance; }

		HLAPI void Reset(T *instance = nullptr)
		{
			DecRef();
			m_Instance = instance;
		}

		template<typename T2>
		HLAPI SharedReference<T2> As() const
		{
			return SharedReference<T2>(*this);
		}

		template<typename... Args>
		HLAPI static SharedReference<T> Create(Args &&...args)
		{
			return SharedReference<T>(new T(std::forward<Args>(args)...));
		}

		HLAPI bool operator==(const SharedReference<T> &other) const
		{
			return m_Instance == other.m_Instance;
		}

		HLAPI bool operator!=(const SharedReference<T> &other) const
		{
			return !(*this == other);
		}

		HLAPI bool Equals(const SharedReference<T> &other) const
		{
			if (!m_Instance || !other.m_Instance)
				return false;

			return *m_Instance == *other.m_Instance;
		}

		HLAPI bool IsValid() const
		{
			return m_Instance != nullptr;
		}

	private:

		void IncRef() const
		{
			if (m_Instance)
				m_Instance->IncrementReferenceCount();
		}

		void DecRef() const
		{
			if (m_Instance)
			{
				m_Instance->DecrementReferenceCount();
				if (m_Instance->GetReferenceCount() == 0)
				{
					delete m_Instance;
				}
			}
		}

		template<class T2>
		friend class SharedReference;
		T* m_Instance;
	};

	template<typename T>
	using Ref = SharedReference<T>;
}
