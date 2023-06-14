// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class SelectionStack
	{
	public:

		HLAPI void CopyFrom(const SelectionStack &other)
		{
			m_Selections.assign(other.begin(), other.end());
		}

		HLAPI void Clear()
		{
			m_Selections.clear();
		}

		HLAPI void Select(AssetHandle handle)
		{
			if (IsSelected(handle))
				return;

			m_Selections.push_back(handle);
		}

		HLAPI void Deselect(AssetHandle handle)
		{
			if (!IsSelected(handle))
				return;

			for (auto it = m_Selections.begin(); it != m_Selections.end(); ++it)
			{
				if (handle == *it)
				{
					m_Selections.erase(it);
					break;
				}
			}
		}

		HLAPI bool IsSelected(AssetHandle handle) const
		{
			for (const auto &selectedHandle : m_Selections)
			{
				if (selectedHandle == handle)
					return true;
			}

			return false;
		}

		HLAPI uint32 Count() const { return (uint32)m_Selections.size(); }
		HLAPI const AssetHandle *Data() const { return m_Selections.data(); }

		HLAPI AssetHandle operator[](uint32 index) const
		{
			HL_ASSERT(index >= 0 && index < m_Selections.size());
			return m_Selections[index];
		}

		HLAPI std::vector<AssetHandle>::iterator begin() { return m_Selections.begin(); }
		HLAPI std::vector<AssetHandle>::const_iterator begin() const { return m_Selections.begin(); }

		HLAPI std::vector<AssetHandle>::iterator end() { return m_Selections.end(); }
		HLAPI std::vector<AssetHandle>::const_iterator end() const { return m_Selections.end(); }

	private:

		std::vector<AssetHandle> m_Selections;
	};
}

