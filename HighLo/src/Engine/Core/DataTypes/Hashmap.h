#pragma once

namespace highlo
{
	template<typename KeyType, typename ValueType>
	class HLHashmap
	{
	private:
		std::vector<std::pair<KeyType, ValueType>> m_Elements;

	public:

		HLAPI HLHashmap() {}
		HLAPI ~HLHashmap()
		{
			m_Elements.clear();
			m_Elements.shrink_to_fit();
		}

		HLAPI bool HasKey(const KeyType &key)
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
			{
				std::pair<KeyType, ValueType> current = *it;
				if (current.first == key)
					return true;
			}

			return false;
		}

		HLAPI bool HasValue(const ValueType &value)
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
			{
				std::pair<KeyType, ValueType> current = *it;
				if (current.second == value)
					return true;
			}

			return false;
		}

		HLAPI bool Put(const KeyType &key, const ValueType &value)
		{
			if (!HasKey(key) && !HasValue(value))
			{
				std::pair<KeyType, ValueType> pair;
				pair.first = key;
				pair.second = value;
				m_Elements.push_back(pair);
				return true;
			}

			return false;
		}

		HLAPI bool Set(const KeyType &key, const ValueType &value)
		{
			if (HasKey(key))
			{
				uint32 i;
				for (i = 0; i < m_Elements.size(); ++i)
				{
					std::pair<KeyType, ValueType> current = m_Elements.at(i);
					if (current.first == key)
					{
						m_Elements.at(i).second = value;
						return true;
					}
				}
			}

			return false;
		}

		HLAPI bool Remove(const KeyType &key, const ValueType &value)
		{
			if (HasKey(key) && HasValue(value))
			{
				uint32 i;
				for (i = 0; i < m_Elements.size(); ++i)
				{
					std::pair<KeyType, ValueType> current = m_Elements.at(i);
					if (current.first == key && current.second == value)
					{
						m_Elements.erase(m_Elements.begin() + i);
						return true;
					}
				}
			}

			return false;
		}

		HLAPI bool Remove(const KeyType &key)
		{
			if (HasKey(key))
			{
				uint32 i;
				for (i = 0; i < m_Elements.size(); ++i)
				{
					std::pair<KeyType, ValueType> current = m_Elements.at(i);
					if (current.first == key)
						m_Elements.erase(m_Elements.begin() + i);
				}

				return true;
			}

			return false;
		}

		HLAPI void RemoveAll()
		{
			m_Elements.clear();
			m_Elements.shrink_to_fit();
		}

		HLAPI bool RemoveFirst()
		{
			m_Elements.erase(m_Elements.begin() + 0);
			return true;
		}

		HLAPI bool RemoveLast()
		{
			m_Elements.erase(m_Elements.begin() + (m_Elements.size() - 1));
			return true;
		}

		HLAPI ValueType &Find(const KeyType &key)
		{
			static ValueType result;

			for (uint32 i = 0; i < m_Elements.size(); ++i)
			{
				if (key == m_Elements[i].first)
				{
					result = m_Elements[i].second;
					break;
				}
			}

			return result;
		}

		HLAPI ValueType &GetFirst()
		{
			return m_Elements.at(0).second;
		}

		HLAPI ValueType &GetLast()
		{
			return m_Elements.at(m_Elements.size() - 1).second;
		}

		HLAPI ValueType &GetAt(int32 i)
		{
			return m_Elements.at(i).second;
		}

		HLAPI KeyType &GetKey(int32 i)
		{
			return m_Elements.at(i).first;
		}

		HLAPI bool IsEmpty()
		{
			return m_Elements.size() == 0;
		}

		HLAPI uint32 Size()
		{
			return (uint32)m_Elements.size();
		}

		HLAPI void Print()
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); it++)
			{
				std::pair<KeyType, ValueType> current = *it;
				std::cout << current.first << ", " << current.second << std::endl;
			}
		}

		HLAPI ValueType &operator[](const KeyType &key)
		{
			static ValueType value;

			for (uint32 i = 0; i < m_Elements.size(); ++i)
			{
				std::pair<KeyType, ValueType> current = m_Elements.at(i);
				if (current.first == key)
				{
					value = current.second;
					break;
				}
			}

			return value;
		}

		HLAPI const ValueType &operator[](const KeyType &key) const
		{
			static ValueType value;

			for (uint32 i = 0; i < m_Elements.size(); ++i)
			{
				std::pair<KeyType, ValueType> current = m_Elements.at(i);
				if (current.first == key)
				{
					value = current.second;
					break;
				}
			}

			return value;
		}

		HLAPI ValueType &operator[](size_t i) { HL_ASSERT(i < m_Elements.size()); return m_Elements.at(i).second; }
		HLAPI const ValueType &operator[](size_t i) const { HL_ASSERT(i < m_Elements.size()); return m_Elements.at(i).second; }

		HLAPI friend std::ostream &operator<<(std::ostream &stream, HLHashmap<KeyType, ValueType> &hashmap)
		{
			stream << "[" << std::endl;
			uint32 i;
			for (i = 0; i < hashmap.Size(); ++i)
			{
				std::pair<KeyType, ValueType> &current = hashmap[i];

				stream << "  {";
				stream << " " << current.first << ", " << current.second << " ";

				if ((i + 1) == hashmap.Size())
					stream << "}" << std::endl;
				else
					stream << "}," << std::endl;
			}
			stream << "]";

			return stream;
		}
	};
}
