// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-05) initial release
//

#pragma once

#include "Engine/Core/SharedReference.h"
#include "Engine/Core/SharedReferenceManager.h"

namespace highlo
{
	template<typename T>
	class WeakRef
	{
	public:

		HLAPI WeakRef() = default;

		HLAPI WeakRef(SharedReference<T> ref)
		{
			m_Instance = ref.Get();
		}

		HLAPI WeakRef(T *instance)
		{
			m_Instance = instance;
		}

		HLAPI T *operator->() { return m_Instance; }
		HLAPI const T *operator->() const { return m_Instance; }

		HLAPI T &operator*() { return *m_Instance; }
		HLAPI const T &operator*() const { return *m_Instance; }

		HLAPI bool IsValid() const { return m_Instance ? SharedReferenceManager::Get()->IsSharedRefAlive((void*)m_Instance) : false; }
		HLAPI operator bool() const { return IsValid(); }

	private:

		T *m_Instance = nullptr;
	};
}

