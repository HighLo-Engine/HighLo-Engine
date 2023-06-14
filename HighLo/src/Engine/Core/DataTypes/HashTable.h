// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-02) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	template<typename T>
	class HashTable
	{
	private:

		struct Item
		{
			HLString Name = "";
			uint64 Hash = 0;
			T *Value = nullptr;

			Item()
			{
				Value = new T{};
			}

			~Item()
			{
				if (Value)
				{
					Value = nullptr;
				}
			}

			Item(const Item&) = default;
			Item &operator=(const Item&) = default;
		};

		Item **m_Items = nullptr;
		uint32 m_Count = 0;
		uint32 m_Capacity = 0;

		uint64 Hash(const HLString &name)
		{
			// A multipler to use when generating a hash. Prime to hopefully avoid collisions.
			static const uint64 multiplier = 97;

			unsigned const char *us;
			uint64 hash = 0;

			for (us = (unsigned const char*)(*name); *us; us++)
			{
				hash = hash * multiplier + *us;
			}

			// Mod it against the size of the table.
			if (m_Count > 0)
				hash %= m_Count;

			return hash;
		}

	public:

		HashTable() = default;
		HashTable(uint32 size)
		{
			m_Capacity = size;
			m_Count = 0;
			m_Items = new Item*[size];
			for (uint32 i = 0; i < size; ++i)
				m_Items[i] = new Item{};
			
		}

		~HashTable()
		{
			for (uint32 i = 0; i < m_Capacity; ++i)
				delete m_Items[i];

			delete[] m_Items;
			m_Items = nullptr;
		}

		void Clear()
		{
			for (uint32 i = 0; i < m_Capacity; ++i)
			{
				Item *item = m_Items[i];
				delete item->Value;
				delete item;
			}

			delete[] m_Items;
			m_Items = nullptr;
		}

		bool Set(const HLString &name, T *value)
		{
			uint64 hash = Hash(name);
			if (!m_Items)
				return false;

			for (uint32 i = 0; i < m_Count; ++i)
			{
				Item *item = m_Items[i];
				if (item->Hash == hash)
				{
					// found an existing key, replace the value
					delete item->Value;
					memcpy(item->Value, value, sizeof(value));
					return true;
				}
			}
			
			if (!m_Items || m_Count >= m_Capacity)
			{
				Resize(m_Capacity * 2);
			}

			Item *item = new Item();
			item->Hash = hash;
			item->Name = name;
			memcpy(item->Value, value, sizeof(value));
			m_Items[m_Count] = item;
			m_Count++;

			return true;
		}

		bool Get(const HLString &name, T *out_value)
		{
			uint64 hash = Hash(name);
			if (!m_Items)
				return false;

			for (uint32 i = 0; i < m_Count; ++i)
			{
				Item *item = m_Items[i];
				if (item->Hash == hash)
				{
					memcpy(out_value, item->Value, sizeof(item->Value));
					return true;
				}
			}

			return false;
		}

		bool Has(const HLString &name)
		{
			uint64 hash = Hash(name);
			if (!m_Items)
				return false;

			for (uint32 i = 0; i < m_Count; ++i)
			{
				Item *item = m_Items[i];
				if (item->Hash == hash)
				{
					return true;
				}
			}

			return false;
		}

		bool Fill(T *value)
		{
			if (!m_Items)
				return false;

			for (uint32 i = 0; i < m_Count; ++i)
			{
				Item *item = m_Items[i];
				if (!item)
					return false;

				memcpy(item->Value, value, sizeof(value));
			}

			return true;
		}

		uint32 Resize(uint32 new_size)
		{
			if (new_size > m_Capacity)
			{
				// Upsize
				Item **new_storage = new Item*[new_size];

				// initialize all new items
				for (uint32 i = m_Capacity; i < new_size; ++i)
					new_storage[i] = new Item{};

				for (uint32 i = 0; i < m_Capacity; ++i)
					memcpy(new_storage[i], m_Items[i], sizeof(m_Items[i]));

				for (uint32 i = 0; i < m_Capacity; ++i)
					delete m_Items[i];

				delete[] m_Items;
				m_Items = new_storage;
				m_Capacity = new_size;
			}
			else
			{
				// Downsize
				Item **new_storage = new Item*[new_size];
				for (uint32 i = 0; i < new_size; ++i)
				{
					memcpy(new_storage[i], m_Items[i], sizeof(m_Items[i]));
				}

				for (uint32 i = 0; i < m_Capacity; ++i)
					delete m_Items[i];

				delete[] m_Items;
				m_Items = new_storage;
				m_Capacity = new_size;
			}

			return m_Capacity;
		}
	};
}

