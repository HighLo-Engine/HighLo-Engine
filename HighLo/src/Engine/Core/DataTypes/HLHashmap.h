#pragma once

namespace highlo
{
	template<typename KeyType, typename ValueType>
	class HLHashmap
	{
	private:
		std::vector<std::pair<KeyType, ValueType>> m_Elements;

	public:

		HLHashmap() {}
		~HLHashmap()
		{
			m_Elements.clear();
			m_Elements.shrink_to_fit();
		}

		bool ContainsKey(KeyType key)
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
			{
				std::pair<KeyType, ValueType> current = *it;
				if (current.first == key)
					return true;
			}

			return false;
		}

		bool ContainsValue(ValueType value)
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
			{
				std::pair<KeyType, ValueType> current = *it;
				if (current.second == value)
					return true;
			}

			return false;
		}

		bool Put(KeyType key, ValueType value)
		{
			if (!ContainsKey(key) && !ContainsValue(value))
			{
				std::pair<KeyType, ValueType> pair;
				pair.first = key;
				pair.second = value;
				m_Elements.push_back(pair);
				return true;
			}

			return false;
		}

		bool Set(KeyType key, ValueType value)
		{
			if (ContainsKey(key))
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

		bool Remove(KeyType key, ValueType value)
		{
			if (ContainsKey(key) && ContainsValue(value))
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

		bool Remove(KeyType key)
		{
			if (ContainsKey(key))
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

		void RemoveAll()
		{
			m_Elements.clear();
			m_Elements.shrink_to_fit();
		}

		bool RemoveFirst()
		{
			m_Elements.erase(m_Elements.begin() + 0);
			return true;
		}

		bool RemoveLast()
		{
			m_Elements.erase(m_Elements.begin() + (m_Elements.size() - 1));
			return true;
		}

		ValueType GetFirst()
		{
			return m_Elements.at(0).second;
		}

		ValueType GetLast()
		{
			return m_Elements.at(m_Elements.size() - 1).second;
		}

		ValueType GetAt(int32 i)
		{
			return m_Elements.at(i).second;
		}

		KeyType GetKey(int32 i)
		{
			return m_Elements.at(i).first;
		}

		bool IsEmpty()
		{
			return m_Elements.size() == 0;
		}

		uint32 Size()
		{
			return (uint32)m_Elements.size();
		}

		void Print()
		{
			for (auto it = m_Elements.begin(); it != m_Elements.end(); it++)
			{
				std::pair<KeyType, ValueType> current = *it;
				std::cout << current.first << ", " << current.second << std::endl;
			}
		}

		bool HasKey(const KeyType &key) const
		{
			for (uint32 i = 0; i < m_Elements.size(); ++i)
			{
				std::pair<KeyType, ValueType> current = m_Elements.at(i);
				if (current.first == key)
					return true;
			}

			return false;
		}

		ValueType &operator[](const KeyType &key)
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

		const ValueType &operator[](const KeyType &key) const
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

		ValueType &operator[](size_t i) { HL_ASSERT(i < m_Elements.size()); return m_Elements.at(i).second; }
		const ValueType &operator[](size_t i) const { HL_ASSERT(i < m_Elements.size()); return m_Elements.at(i).second; }

		friend std::ostream &operator<<(std::ostream &stream, HLHashmap<KeyType, ValueType> &hashmap)
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