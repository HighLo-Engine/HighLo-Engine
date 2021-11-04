// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

namespace highlo
{
	struct AssetBrowserItemList
	{
		static constexpr uint32 InvalidItem = std::numeric_limits<uint32>::max();

		std::vector<Ref<AssetBrowserBaseItem>> Items;
		HLAPI std::vector<Ref<AssetBrowserBaseItem>>::iterator begin() { return Items.begin(); }
		HLAPI std::vector<Ref<AssetBrowserBaseItem>>::const_iterator begin() const { return Items.begin(); }
		HLAPI std::vector<Ref<AssetBrowserBaseItem>>::iterator end() { return Items.end(); }
		HLAPI std::vector<Ref<AssetBrowserBaseItem>>::const_iterator end() const { return Items.end(); }

		HLAPI Ref<AssetBrowserBaseItem> &operator[](uint32 index) { HL_ASSERT(index >= 0 && index < Items.size()); return Items[index]; }
		HLAPI const Ref<AssetBrowserBaseItem> &operator[](uint32 index) const { HL_ASSERT(index >= 0 && index < Items.size());  return Items[index]; }

		HLAPI void Clear()
		{
			Items.clear();
		}

		HLAPI uint32 Find(AssetHandle handle) const
		{
			for (uint32 i = 0; i < Items.size(); ++i)
			{
				if (Items[i]->GetID() == handle)
					return i;
			}

			return InvalidItem;
		}

		HLAPI void Erase(AssetHandle handle)
		{
			uint32 index = Find(handle);
			if (index == InvalidItem)
				return;

			auto it = Items.begin() + index;
			Items.erase(it);
		}
	};
}

