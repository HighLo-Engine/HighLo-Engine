// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "String.h"

namespace highlo
{
	HLString::HLString(const char *data)
	{
		m_Size = (uint32) strlen(data);
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, data, m_Size);
	}

	HLString::HLString(const char *data, uint32 length)
	{
		m_Size = length;
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, data, m_Size);
	}

	HLString::HLString(const wchar_t *data)
	{
		m_Size = (uint32) wcslen(data);
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, data, m_Size);
	}

	HLString::HLString(const std::string &str)
	{
		m_Size = (uint32) strlen(str.c_str());
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, str.c_str(), m_Size);
	}

	HLString::HLString(const std::wstring &wideStr)
	{
		m_Size = (uint32) wcslen(wideStr.c_str());
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, wideStr.c_str(), m_Size);
	}

	HLString::HLString(const HLString &other)
	{
		m_Size = other.m_Size;
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, other.m_Data, m_Size);
	}

	HLString::HLString(const HLString &other, uint32 length)
	{
		m_Size = length;
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, other.m_Data, m_Size);
	}

	HLString::HLString(const HLString &other, uint32 start, uint32 end)
	{
		m_Size = end - start;
		m_Data = new char[m_Size + 1];
		m_Data[m_Size] = '\0';
		memcpy(m_Data, &other.m_Data[start], m_Size);
	}

	HLString::HLString(HLString &&other) noexcept
	{
		m_Size = other.m_Size;
		m_Data = other.m_Data;

		other.m_Size = 0;
		other.m_Data = nullptr;
	}

	HLString::~HLString()
	{
		if (m_Data)
		{
			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
		}
	}

	HLString &HLString::operator=(const HLString &other)
	{
		if (this != &other)
		{
			if (m_Data)
				delete[] m_Data;

			m_Size = other.m_Size;
			m_Data = new char[m_Size + 1];
			m_Data[m_Size] = '\0';
			memcpy(m_Data, other.m_Data, m_Size);
		}

		return *this;
	}

	HLString &HLString::operator=(HLString &&other) noexcept
	{
		if (this != &other)
		{
			m_Size = other.m_Size;
			m_Data = other.m_Data;

			other.m_Size = 0;
			other.m_Data = nullptr;
		}

		return *this;
	}

	void HLString::Clear()
	{
		delete[] m_Data;
		m_Data = nullptr;
		m_Size = 0;
	}

	void HLString::Resize(uint32 size)
	{
		if (m_Data)
			delete[] m_Data;

		m_Size = size;
		m_Data = new char[size];
	}

	uint32 HLString::Length() const
	{
		return m_Size;
	}

	wchar_t *HLString::W_Str()
	{
		wchar_t *result = new wchar_t[m_Size + 1];
		for (uint32 i = 0; i < m_Size; ++i)
			result[i] = m_Data[i];

		result[m_Size] = '\0';
		return result;
	}

	const wchar_t *HLString::W_Str() const
	{
		wchar_t *result = new wchar_t[m_Size + 1];
		for (uint32 i = 0; i < m_Size; ++i)
			result[i] = m_Data[i];

		result[m_Size] = '\0';
		return result;
	}

	char *HLString::C_Str()
	{
		return m_Data;
	}

	const char *HLString::C_Str() const
	{
		return m_Data;
	}

	char HLString::At(uint32 index)
	{
		if (index < m_Size)
		{
			return m_Data[index];
		}

		return (char) NPOS;
	}

	const char HLString::At(uint32 index) const
	{
		if (index < m_Size)
		{
			return m_Data[index];
		}

		return (char) NPOS;
	}

	HLString &HLString::Append(const char letter)
	{
		char *new_data = new char[m_Size + 2]; // +1 for a character and another +1 for the null terminator
		new_data[m_Size] = letter;
		new_data[m_Size + 1] = '\0';

		if (m_Data)
		{
			memcpy(new_data, m_Data, m_Size);
			delete[] m_Data;
		}

		m_Data = new_data;
		++m_Size;

		return *this;
	}

	HLString &HLString::Append(const HLString &other)
	{
		uint32 new_size = m_Size + other.m_Size;
		char *new_data = new char[new_size + 1]; // +1 for the null terminator
		new_data[new_size] = '\0';

		if (m_Data)
		{
			memcpy(new_data, m_Data, m_Size); // copy existing string m_Data
			delete[] m_Data;
		}

		memcpy((char *) (new_data + m_Size), other.m_Data, other.m_Size); // copy appended string m_Data
		m_Data = new_data;
		m_Size = new_size;

		return *this;
	}

	HLString &HLString::Remove(const char letter)
	{
		if (Contains(letter))
			{
			uint32 i = 0;
			uint32 j = 0;
			uint32 start_pos = IndexOf(letter);
			uint32 new_size = m_Size - 1;
			char *new_data = new char[new_size + 1];
			new_data[new_size] = '\0';

			if (m_Data)
			{
				if (start_pos > 0)
				{
					memcpy(new_data, m_Data, start_pos);
					j = start_pos;
				}

				for (i = start_pos + 1; i < m_Size; ++i)
				{
					new_data[j] = m_Data[i];
					++j;
				}

				delete[] m_Data;
				m_Data = new_data;
				m_Size = new_size;
			}
		}

		return *this;
	}

	HLString &HLString::Remove(const HLString &other)
	{
		if (Contains(other))
		{
			uint32 i = 0;
			uint32 j = 0;
			uint32 start_pos = IndexOf(other);
			uint32 new_size = m_Size - other.Length();
			char *new_data = new char[new_size + 1];
			new_data[new_size] = '\0';

			if (m_Data)
			{
				// copy part before the part to remove
				if (start_pos > 0)
				{
					memcpy(new_data, m_Data, start_pos);
					j = start_pos;
				}

				// copy part after the part to remove
				for (i = start_pos + other.Length(); i < m_Size; ++i)
				{
					new_data[j] = m_Data[i];
					++j;
				}

				delete[] m_Data;
				m_Data = new_data;
				m_Size = new_size;
			}
		}

		return *this;
	}

	uint32 HLString::FirstIndexOf(const char letter, uint32 offset) const
	{
		for (uint32 i = offset; i < m_Size; ++i)
		{
			if (m_Data[i] == letter)
				return i;
		}

		return NPOS;
	}

	uint32 HLString::FirstIndexOf(const HLString &other, uint32 offset) const
	{
		uint32 i = offset;
		uint32 j = 0;

		while (m_Data[i] != '\0')
		{
			if (m_Data[i] == other[j])
			{
				while (m_Data[i] == other[j] && other[j] != '\0')
				{
					++j;
					++i;
				}

				if (other[j] == '\0')
					return i - j;

				j = 0;
			}

			++i;
		}

		return NPOS;
	}

	uint32 HLString::IndexOf(const char letter, uint32 offset) const
	{
		for (uint32 i = offset; i < m_Size; ++i)
		{
			if (m_Data[i] == letter)
				return i;
		}

		return NPOS;
	}

	uint32 HLString::IndexOf(const HLString &other, uint32 offset) const
	{
		uint32 i = offset;
		uint32 j = 0;

		while (m_Data[i] != '\0')
		{
			if (m_Data[i] == other[j])
			{
				while (m_Data[i] == other[j] && other[j] != '\0')
				{
					++j;
					++i;
				}

				if (other[j] == '\0')
					return i - j;

				j = 0;
			}

			++i;
		}

		return NPOS;
	}

	uint32 HLString::LastIndexOf(const char letter, uint32 offset) const
	{
		HL_ASSERT(offset >= 0 && offset < m_Size, "Offset is out of bounds!");

		if (offset == 0)
			offset = m_Size;

		// Count how many times the letter exists
		uint32 letterCount = 0;
		for (uint32 i = 0; i < offset; ++i)
		{
			if (m_Data[i] == letter)
				++letterCount;
		}

		// If No Letter exists, return
		if (0 == letterCount)
			return NPOS;

		// Get the last letter index
		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (letterCount == 1 && m_Data[i] == letter)
			{
				return i;
			}

			if (m_Data[i] == letter && letterCount > 0)
			{
				--letterCount;
				continue;
			}
		}

		return NPOS;
	}

	uint32 HLString::LastIndexOf(const HLString &other, uint32 offset) const
	{
		if (Contains(other))
		{
			uint32 i = m_Size - other.Length();
			uint32 j = 0;

			while (m_Data[i] != '\0')
			{
				if (m_Data[i] == other[j])
				{
					while (m_Data[i] == other[j] && other[j] != '\0')
					{
						++j;
						++i;
					}

					if (other[j] == '\0')
						return i - j;

					j = 0;
				}

				++i;
			}

			return i;
		}

		return NPOS;
	}

	uint32 HLString::FirstIndexNotOf(const char letter, uint32 offset) const
	{
		const int len = static_cast<int>(strspn(m_Data + offset, &letter));
		if (len + offset >= m_Size)
			return NPOS;

		return len + offset;
	}

	uint32 HLString::FirstIndexNotOf(const HLString &other, uint32 offset) const
	{
		const int len = static_cast<int>(strspn(m_Data + offset, *other));
		if (len + offset >= m_Size)
			return NPOS;

		return len + offset;
	}

	std::vector<HLString> HLString::Split(char delimiter)
	{
		uint32 wordCount = 1;
		uint32 charIdx = 0;
		uint32 wordBeginIdx = 0;
		HLString word;

		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == delimiter)
				++wordCount;
		}

		std::vector<HLString> result(wordCount);
		for (uint32 i = 0; i < wordCount; ++i)
		{
			wordBeginIdx = charIdx;
			while (m_Data[charIdx] != delimiter && m_Data[charIdx] != '\0')
				++charIdx;

			word = Substr(wordBeginIdx, charIdx);
			result[i] = word;
			++charIdx;
		}

		return result;
	}

	HLString *HLString::Split(char delimiter, uint32 *outWordCount)
	{
		uint32 wordCount = 1;
		uint32 charIdx = 0;
		uint32 wordBeginIdx = 0;
		HLString word;

		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == delimiter)
				++wordCount;
		}

		HLString *result = new HLString[wordCount];
		for (uint32 i = 0; i < wordCount; ++i)
		{
			wordBeginIdx = charIdx;
			while (m_Data[charIdx] != delimiter && m_Data[charIdx] != '\0')
				++charIdx;

			word = Substr(wordBeginIdx, charIdx);
			result[i] = word;
			++charIdx;
		}

		if (outWordCount)
			*outWordCount = wordCount;

		return result;
	}

	HLString &HLString::Replace(const HLString &find, const HLString &replaceValue, uint32 occurencesToReplace)
	{
		uint32 occurences = 0;
		uint32 offset = 0;

		while (Contains(find, offset))
		{
			++occurences;
			offset = IndexOf(find, offset) + find.Length();

			if (occurencesToReplace && occurences == occurencesToReplace)
				break;
		}

		if (!occurences)
			return *this;

		uint32 *occurence_indices = new uint32[occurences];
		offset = 0;
		for (uint32 i = 0; i < occurences; ++i)
		{
			occurence_indices[i] = IndexOf(find, offset);
			offset = occurence_indices[i] + find.Length();
		}

		uint32 new_size = m_Size + ((replaceValue.Length() - find.Length()) * occurences);
		char *new_data = new char[new_size + 1];
		new_data[new_size] = '\0';

		uint32 occurence_idx = 0;
		for (uint32 dataIdx = 0, NewDataIdx = 0; dataIdx < m_Size;)
		{
			if (dataIdx != occurence_indices[occurence_idx])
			{
				new_data[NewDataIdx] = m_Data[dataIdx];
				++NewDataIdx;
				++dataIdx;
			}
			else
			{
				memcpy(new_data + NewDataIdx, replaceValue.m_Data, replaceValue.Length());
				NewDataIdx += replaceValue.Length();
				dataIdx += find.Length();
				++occurence_idx;
			}
		}

		if (m_Data)
			delete[] m_Data;

		m_Data = new_data;
		m_Size = new_size;
		delete[] occurence_indices;

		return *this;
	}

	HLString &HLString::Reverse()
	{
		for (uint32 i = 0; i < m_Size / 2; i++)
		{
			char temp = m_Data[i];
			m_Data[i] = m_Data[m_Size - i - 1];
			m_Data[m_Size - i - 1] = temp;
		}

		return *this;
	}

	HLString HLString::Substr(uint32 beginIndex, uint32 endIndex) const
	{
		if (endIndex == 0)
			endIndex = m_Size;

		if ((endIndex - beginIndex) > m_Size)
			return HLString("-1");

		return HLString(*this, beginIndex, endIndex);
	}

	const HLString &HLString::ToLowerCase() const
	{
		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] >= 'A' && m_Data[i] <= 'Z')
				m_Data[i] = m_Data[i] - ('A' - 'a');
		}

		return *this;
	}

	const HLString &HLString::ToUpperCase() const
	{
		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] >= 'a' && m_Data[i] <= 'z')
				m_Data[i] = m_Data[i] + ('A' - 'a');
		}

		return *this;
	}

	void HLString::Print() const
	{
		for (uint32 i = 0; i < m_Size; ++i)
			printf("%c", m_Data[i]);
		printf("\n");
	}

	uint64 HLString::Hash() const
	{
		uint64 hash = 0;
		const uint32 g = 42;
		uint32 i = 0;

		for (i = 0; i < m_Size; ++i)
			hash = g * hash + m_Data[i];

		return hash;
	}

	bool HLString::IsEmpty() const
	{
		return m_Size == 0;
	}

	bool HLString::Contains(const char letter, uint32 offset) const
	{
		bool hasLetter = false;

		for (uint32 i = offset; i < m_Size; ++i)
		{
			if (m_Data[i] == letter)
			{
				hasLetter = true;
				break;
			}
		}

		return hasLetter;
	}

	bool HLString::Contains(const HLString &other, uint32 offset) const
	{
		uint32 i = offset;
		uint32 j = 0;

		while (m_Data[i] != '\0')
		{
			if (m_Data[i] == other[j])
			{
				while (m_Data[i] == other[j] && other[j] != '\0')
				{
					++j;
					++i;
				}

				if (other[j] == '\0')
					return true;

				j = 0;
			}

			++i;
		}

		return false;
	}

	bool HLString::StartsWith(const char letter) const
	{
		if (m_Data[0] == letter)
			return true;

		return false;
	}

	bool HLString::StartsWith(const HLString &other) const
	{
		uint32 equalCount = 0;
		for (uint32 i = 0; i < other.m_Size; ++i)
		{
			if (m_Data[i] == other[i])
				++equalCount;
		}

		if (equalCount == other.m_Size)
			return true;

		return false;
	}

	bool HLString::EndsWith(const char letter) const
	{
		if (m_Data[m_Size - 1] == letter)
			return true;

		return false;
	}

	bool HLString::EndsWith(const HLString &other) const
	{
		uint32 equalCount = 0;
		uint32 start_pos = m_Size - other.m_Size;

		for (uint32 i = start_pos; i < m_Size; ++i)
		{
			for (uint32 j = 0; j < other.m_Size; ++j)
			{
				if (m_Data[i] == other[j])
					++equalCount;
			}
		}

		if (equalCount == other.m_Size)
			return true;

		return false;
	}

	char *HLString::operator*()
	{
		return m_Data;
	}

	const char *HLString::operator*() const
	{
		return m_Data;
	}

	HLString::operator char *()
	{
		return m_Data;
	}

	HLString::operator const char *() const
	{
		return m_Data;
	}

	bool HLString::operator==(const char *other) const
	{
		if (m_Size != strlen(other))
			return false;

		uint32 equalCount = 0;
		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == other[i])
				++equalCount;
		}

		if (equalCount == m_Size)
			return true;

		return false;
	}

	bool HLString::operator==(const HLString &other) const
	{
		if (m_Size != other.m_Size)
			return false;

		uint32 equalCount = 0;
		for (uint32 i = 0; i < m_Size; ++i)
		{
			if (m_Data[i] == other[i])
				++equalCount;
		}

		if (equalCount == m_Size)
			return true;

		return false;
	}

	bool HLString::operator!=(const char *other) const
	{
		return !(*this == other);
	}

	bool HLString::operator!=(const HLString &other) const
	{
		return !(*this == other);
	}

	bool HLString::operator<(const char *other) const
	{
		return m_Size < strlen(other);
	}

	bool HLString::operator<(const HLString &other) const
	{
		return m_Size < other.m_Size;
	}

	bool HLString::operator>(const char *other) const
	{
		return m_Size > strlen(other);
	}

	bool HLString::operator>(const HLString &other) const
	{
		return m_Size > other.m_Size;
	}

	bool HLString::operator<=(const char *other) const
	{
		return m_Size <= strlen(other);
	}

	bool HLString::operator<=(const HLString &other) const
	{
		return m_Size <= other.m_Size;
	}

	bool HLString::operator>=(const char *other) const
	{
		return m_Size >= strlen(other);
	}

	bool HLString::operator>=(const HLString &other) const
	{
		return m_Size >= other.m_Size;
	}

	HLString &HLString::operator-=(const HLString &other)
	{
		return Remove(other);
	}

	HLString &HLString::operator-=(const char letter)
	{
		return Remove(letter);
	}

	HLString &HLString::operator+=(const HLString &other)
	{
		return Append(other);
	}

	HLString &HLString::operator+=(const char letter)
	{
		return Append(letter);
	}

	char &HLString::operator[](uint32 index)
	{
		HL_ASSERT(index <= m_Size);
		return m_Data[index];
	}

	const char &HLString::operator[](uint32 index) const
	{
		HL_ASSERT(index <= m_Size);
		return m_Data[index];
	}

	HLString operator-(HLString str, const HLString &other)
	{
		return str.Remove(other);
	}

	HLString operator-(HLString str, const char letter)
	{
		return str.Remove(letter);
	}

	HLString operator-(HLString str, const char *other)
	{
		return str.Remove(other);
	}

	HLString operator+(HLString str, const HLString &other)
	{
		return str.Append(other);
	}

	HLString operator+(HLString str, const char letter)
	{
		return str.Append(letter);
	}

	HLString operator+(HLString str, const char *other)
	{
		return str.Append(other);
	}

	std::ostream &operator<<(std::ostream &stream, const HLString &str)
	{
		for (uint32 i = 0; i < str.m_Size; ++i)
			stream << str.m_Data[i];

		return stream;
	}
}
