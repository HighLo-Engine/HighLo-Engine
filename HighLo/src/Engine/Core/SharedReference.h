// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

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

		SharedReference()
			: m_Instance(nullptr) {}

		SharedReference(std::nullptr_t)
			: m_Instance(nullptr) {}

		SharedReference(T* instance)
			: m_Instance(instance)
		{
			static_assert(std::is_base_of<IsSharedReference, T>::value, "Class is not RefCounted!");
			IncRef();
		}

		template<typename T2>
		SharedReference(const SharedReference<T2>& other)
		{
			m_Instance = (T*)other.m_Instance;
			IncRef();
		}

		template<typename T2>
		SharedReference(SharedReference<T2>&& other)
		{
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
		}

		~SharedReference()
		{
			DecRef();
		}

		SharedReference(const SharedReference<T>& other)
			: m_Instance(other.m_Instance)
		{
			IncRef();
		}

		SharedReference& operator=(std::nullptr_t)
		{
			DecRef();
			m_Instance = nullptr;
			return *this;
		}

		SharedReference& operator=(const SharedReference<T>& other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		SharedReference& operator=(const SharedReference<T2>& other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		SharedReference& operator=(SharedReference<T2>&& other)
		{
			DecRef();

			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}

		operator bool() { return m_Instance != nullptr; }
		operator bool() const { return m_Instance != nullptr; }

		T* operator->() { return m_Instance; }
		const T* operator->() const { return m_Instance; }

		T& operator*() { return *m_Instance; }
		const T& operator*() const { return *m_Instance; }

		T* Get() { return  m_Instance; }
		const T* Get() const { return  m_Instance; }

		void Reset(T* instance = nullptr)
		{
			DecRef();
			m_Instance = instance;
		}

		template<typename T2>
		SharedReference<T2> As() const
		{
			return SharedReference<T2>(*this);
		}

		template<typename... Args>
		static SharedReference<T> Create(Args&&... args)
		{
			return SharedReference<T>(new T(std::forward<Args>(args)...));
		}

		bool operator==(const SharedReference<T>& other) const
		{
			return m_Instance == other.m_Instance;
		}

		bool operator!=(const SharedReference<T>& other) const
		{
			return !(*this == other);
		}

		bool Equals(const SharedReference<T>& other)
		{
			if (!m_Instance || !other.m_Instance)
				return false;

			return *m_Instance == *other.m_Instance;
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
