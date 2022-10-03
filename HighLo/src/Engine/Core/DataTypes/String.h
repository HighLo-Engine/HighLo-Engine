// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-09-14) Added SSO (small string optimization)
//     - 1.3 (2021-10-02) Added begin and end functions to be able to use ForEach loops over Strings
//     - 1.2 (2021-10-01) fixed Bug in Contains methods - the engine does not longer crash if the checked string is empty
//     - 1.1 (2021-09-29) Refactored hole class to support multibyte Strings as well
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Defines/BaseTypes.h"
#include "Engine/Utils/StringUtils.h"

#include <iostream>

#define HL_STRING_MAX_STACK_STRING_SIZE 16

namespace highlo
{
	template<typename T>
	class HLStringBase;

	using HLString = HLStringBase<char>;
	using HLString16 = HLStringBase<char16_t>;
	using HLString32 = HLStringBase<char32_t>;
	using HLStringWide = HLStringBase<wchar_t>;

	template<typename StringType>
	class HLStringBase
	{
	private:

		StringType *m_Data = nullptr;
		StringType m_StaticData[HL_STRING_MAX_STACK_STRING_SIZE] = { 0 };
		uint32 m_Size = 0;

	private:

		inline StringType *SelectDataSource(uint32 count)
		{
			if (count < HL_STRING_MAX_STACK_STRING_SIZE)
				return m_StaticData;

			return m_Data;
		}

	public:

		static const uint32 NPOS = static_cast<uint32>(-1);

		HLAPI HLStringBase() = default;

		HLAPI HLStringBase(const StringType *data)
		{
			Assign(data, utils::GetSizeOfUnknownStringType<StringType>(data));
		}

		HLAPI HLStringBase(const StringType *data, uint32 length)
		{
			Assign(data, length);
		}

		HLAPI HLStringBase(const std::string &str)
		{
			Assign(str.c_str(), (uint32)str.size());
		}

		HLAPI HLStringBase(const std::wstring &wideStr)
		{
			Assign(wideStr.c_str(), (uint32)wideStr.size());
		}

		HLAPI HLStringBase(const HLStringBase &other)
		{
			Assign(other.C_Str(), other.Length());
		}

		HLAPI HLStringBase(const HLStringBase &other, uint32 length)
		{
			Assign(other.C_Str(), length);
		}

		HLAPI HLStringBase(const HLStringBase &other, uint32 start, uint32 end)
		{
			Assign(other.C_Str(), end, start);
		}

		HLAPI HLStringBase(HLStringBase &&other) noexcept
		{
			m_Data = other.m_Data;
			m_Size = other.m_Size;

		//	for (uint32 i = 0; i < m_Size; ++i)
		//		m_StaticData[i] = other.m_StaticData[i];

			other.m_Size = 0;
			other.m_Data = nullptr;
		}

		HLAPI ~HLStringBase()
		{
			Clear();
		}

		HLAPI HLStringBase &Assign(const StringType *str, uint32 size, uint32 startOffset = 0)
		{
			if (size == 0 || startOffset > size)
				return *this;

			// In either case we need to clear the previous data
			Clear();

			// If the size of the string is less than 16, move the data into the stack allocated buffer
			if ((size - startOffset) < HL_STRING_MAX_STACK_STRING_SIZE)
			{
				m_Size = size - startOffset;
				memcpy_s(m_StaticData, HL_STRING_MAX_STACK_STRING_SIZE, str + startOffset, m_Size);
				m_StaticData[m_Size] = '\0';

				return *this;
			}

			m_Size = size - startOffset;
			m_Data = new StringType[m_Size + 1];
			m_Data[m_Size] = '\0';
			
			memcpy(m_Data, str + startOffset, m_Size);

			return *this;
		}

		HLAPI HLStringBase &operator=(const HLStringBase &other)
		{
			if (this != &other)
			{
				Assign(other.C_Str(), other.Length());
			}

			return *this;
		}

		HLAPI HLStringBase &operator=(HLStringBase &&other) noexcept
		{
			if (this != &other)
			{
				m_Data = other.m_Data;
				m_Size = other.m_Size;
				
			//	for (uint32 i = 0; i < m_Size; ++i)
			//		m_StaticData[i] = other.m_StaticData[i];

				other.m_Size = 0;
				other.m_Data = nullptr;
			}

			return *this;
		}

		HLAPI StringType *begin()
		{
			StringType *source = SelectDataSource(m_Size);
			return source[0];
		}

		HLAPI StringType *end()
		{
			StringType *source = SelectDataSource(m_Size);
			return source[m_Size];
		}

		HLAPI void Clear()
		{
			for (uint32 i = 0; i < m_Size; ++i)
			{
				m_StaticData[i] = '\0';
			}

			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
		}

		HLAPI void Resize(uint32 size)
		{
			if (m_Data)
				delete[] m_Data;

			m_Size = size;
			m_Data = new StringType[size + 1];
			m_Data[m_Size] = '\0';
		}

		HLAPI uint32 Length() const
		{
			return m_Size;
		}

		HLAPI StringType *Data()
		{
		//	if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
		//		return m_StaticData;

			return m_Data;
		}

		HLAPI const wchar_t *W_Str() const
		{
			wchar_t *result = new wchar_t[m_Size + 1];
			const StringType *source = C_Str();

			for (uint32 i = 0; i < m_Size; ++i)
				result[i] = (wchar_t)source[i];

			result[m_Size] = '\0';
			return result;
		}

		HLAPI const StringType *C_Str() const
		{
			if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
				return m_StaticData;

			return m_Data;
		}

		HLAPI const StringType At(uint32 index) const
		{
			if (index < m_Size)
			{
				if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
					return m_StaticData[index];

				return m_Data[index];
			}

			return (char)NPOS;
		}

		HLAPI HLStringBase &Append(const StringType letter)
		{
			if ((m_Size + 1) < HL_STRING_MAX_STACK_STRING_SIZE)
			{
				// We want to store the data on the stack and we know, that we have enough space for the one character
				m_StaticData[m_Size] = letter;
				++m_Size;
				return *this;
			}

			StringType *new_data = new StringType[m_Size + 2]; // +1 for a character and another +1 for the null terminator
			new_data[m_Size] = letter;
			new_data[m_Size + 1] = '\0';

			if (m_Data)
			{
				memcpy(new_data, m_Data, m_Size);
				delete[] m_Data;
			}
			else if (m_StaticData)
			{
				memcpy(new_data, m_StaticData, m_Size);
				for (uint32 i = 0; i < m_Size; ++i)
				{
					m_StaticData[i] = '\0';
				}
			}

			Assign(new_data, m_Size + 1);
			return *this;
		}

		HLAPI HLStringBase &Append(const HLStringBase &other)
		{
			if ((m_Size + other.Length()) < HL_STRING_MAX_STACK_STRING_SIZE)
			{
				// We want to store the data on the stack and we know, that we have enough space for the other string
				for (uint32 i = m_Size; i < m_Size + other.Length(); ++i)
				{
					m_StaticData[i] = other.At(i);
				}
				
				m_Size += other.Length();
				return *this;
			}

			uint32 new_size = m_Size + other.m_Size;
			StringType *new_data = new StringType[new_size + 1]; // +1 for the null terminator
			new_data[new_size] = '\0';

			if (m_Data)
			{
				memcpy(new_data, m_Data, m_Size); // copy existing string m_Data
				delete[] m_Data;
			}
			else if (m_StaticData)
			{
				memcpy(new_data, m_StaticData, m_Size);
				for (uint32 i = 0; i < m_Size; ++i)
				{
					m_StaticData[i] = '\0';
				}
			}

			memcpy((StringType*)(new_data + m_Size), other.m_Data, other.m_Size); // copy appended string m_Data
			Assign(new_data, new_size);
			return *this;
		}

		HLAPI HLStringBase &Remove(const StringType letter)
		{
			if (Contains(letter))
			{
				uint32 i = 0;
				uint32 j = 0;
				uint32 start_pos = IndexOf(letter);
				uint32 new_size = m_Size - 1;
				StringType *new_data = new StringType[new_size + 1];
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

					Assign(new_data, new_size);
				}
			}

			return *this;
		}

		HLAPI HLStringBase &Remove(const HLStringBase &other)
		{
			if (Contains(other))
			{
				uint32 i = 0;
				uint32 j = 0;
				uint32 start_pos = IndexOf(other);
				uint32 new_size = m_Size - other.Length();
				StringType *new_data = new StringType[new_size + 1];
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

					Assign(new_data, new_size);
				}
			}

			return *this;
		}

		HLAPI uint32 FirstIndexOf(const StringType letter, uint32 offset = 0) const
		{
			const StringType *source = C_Str();
			for (uint32 i = offset; i < m_Size; ++i)
			{
				if (source[i] == letter)
					return i;
			}

			return NPOS;
		}

		HLAPI uint32 FirstIndexOf(const HLStringBase &other, uint32 offset = 0) const
		{
			uint32 i = offset;
			uint32 j = 0;
			const StringType *source = C_Str();

			while (source[i] != '\0')
			{
				if (source[i] == other[j])
				{
					while (source[i] == other[j] && other[j] != '\0')
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

		HLAPI uint32 IndexOf(const StringType letter, uint32 offset = 0) const
		{
			const StringType *source = C_Str();

			for (uint32 i = offset; i < m_Size; ++i)
			{
				if (source[i] == letter)
					return i;
			}

			return NPOS;
		}

		HLAPI uint32 IndexOf(const HLStringBase &other, uint32 offset = 0) const
		{
			uint32 i = offset;
			uint32 j = 0;
			const StringType *source = C_Str();

			while (source[i] != '\0')
			{
				if (source[i] == other[j])
				{
					while (source[i] == other[j] && other[j] != '\0')
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

		HLAPI uint32 LastIndexOf(const StringType letter, uint32 offset = 0) const
		{
			const StringType *source = C_Str();
			HL_ASSERT(offset >= 0 && offset < m_Size, "Offset is out of bounds!");

			if (offset == 0)
				offset = m_Size;

			// Count how many times the letter exists
			uint32 letterCount = 0;
			for (uint32 i = 0; i < offset; ++i)
			{
				if (source[i] == letter)
					++letterCount;
			}

			// If No Letter exists, return
			if (0 == letterCount)
				return NPOS;

			// Get the last letter index
			for (uint32 i = 0; i < m_Size; ++i)
			{
				if (letterCount == 1 && source[i] == letter)
					return i;

				if (source[i] == letter && letterCount > 0)
				{
					--letterCount;
					continue;
				}
			}

			return NPOS;
		}

		HLAPI uint32 LastIndexOf(const HLStringBase &other, uint32 offset = 0) const
		{
			if (Contains(other))
			{
				uint32 i = m_Size - other.Length();
				uint32 j = 0;
				const StringType *source = C_Str();

				while (source[i] != '\0')
				{
					if (source[i] == other[j])
					{
						while (source[i] == other[j] && other[j] != '\0')
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

		HLAPI uint32 FirstIndexNotOf(const StringType letter, uint32 offset = 0) const
		{
			StringType *source = SelectDataSource(m_Size);
			const int32 len = static_cast<int>(strspn(source + offset, &letter));

			if (len + offset >= m_Size)
				return NPOS;

			return len + offset;
		}

		HLAPI uint32 FirstIndexNotOf(const HLStringBase &other, uint32 offset = 0) const
		{
			StringType *source = SelectDataSource(m_Size);
			const int32 len = static_cast<int>(strspn(source + offset, *other));
			
			if (len + offset >= m_Size)
				return NPOS;

			return len + offset;
		}

		HLAPI std::vector<HLStringBase> Split(StringType delimiter)
		{
			uint32 wordCount = 1;
			uint32 charIdx = 0;
			uint32 wordBeginIdx = 0;
			HLStringBase word;
			StringType *source = SelectDataSource(m_Size);

			for (uint32 i = 0; i < m_Size; ++i)
			{
				if (source[i] == delimiter)
					++wordCount;
			}

			std::vector<HLStringBase> result(wordCount);
			for (uint32 i = 0; i < wordCount; ++i)
			{
				wordBeginIdx = charIdx;
				while (source[charIdx] != delimiter && source[charIdx] != '\0')
					++charIdx;

				word = Substr(wordBeginIdx, charIdx);
				result[i] = word;
				++charIdx;
			}

			return result;
		}

		HLAPI HLStringBase *Split(StringType delimiter, uint32 *outWordCount)
		{
			uint32 wordCount = 1;
			uint32 charIdx = 0;
			uint32 wordBeginIdx = 0;
			HLStringBase word;
			StringType *source = SelectDataSource(m_Size);

			for (uint32 i = 0; i < m_Size; ++i)
			{
				if (source[i] == delimiter)
					++wordCount;
			}

			HLStringBase *result = new HLStringBase[wordCount];
			for (uint32 i = 0; i < wordCount; ++i)
			{
				wordBeginIdx = charIdx;
				while (source[charIdx] != delimiter && source[charIdx] != '\0')
					++charIdx;

				word = Substr(wordBeginIdx, charIdx);
				result[i] = word;
				++charIdx;
			}

			if (outWordCount)
				*outWordCount = wordCount;

			return result;
		}

		HLAPI HLStringBase &Replace(const HLStringBase &find, const HLStringBase &replaceValue, uint32 occurencesToReplace = 0)
		{
			// @See https://stackoverflow.com/a/32413923/12873837

			StringType *newString = new StringType[m_Size + 512];
			StringType *strValuePointer = &newString[0];
			const StringType *tmp = m_Data;
			uint32 findLength = find.Length();
			uint32 replaceLength = replaceValue.Length();
			uint32 occurencesReplaced = 0;

			newString[strlen(newString)] = '\0';

			while (true)
			{
				const StringType *p = strstr(tmp, find);
			//	std::cout << "Found substring: " << p << std::endl;

				// walked past last occurrence of needle; copy remaining part
				if (!p)
				{
				//	std::cout << "last part: " << tmp << std::endl;
					strcpy(strValuePointer, tmp);
					break;
				}

				// copy part before needle
				memcpy(strValuePointer, tmp, p - tmp);
				strValuePointer += p - tmp;

				// copy replacement string
				memcpy(strValuePointer, replaceValue, replaceLength);
				strValuePointer += replaceLength;

				// adjust pointers, move on
				tmp = p + findLength;

				++occurencesReplaced;
				if (occurencesToReplace && occurencesReplaced >= occurencesToReplace)
					break;
			}

			Assign(newString, utils::GetSizeOfUnknownStringType<StringType>(newString));
			return *this;
		}

		HLAPI HLStringBase &Reverse()
		{
			StringType *source = SelectDataSource(m_Size);
			for (uint32 i = 0; i < m_Size / 2; i++)
			{
				StringType temp = source[i];
				source[i] = source[m_Size - i - 1];
				source[m_Size - i - 1] = temp;
			}

			return *this;
		}

		HLAPI HLStringBase &Substr(uint32 beginIndex, uint32 endIndex = 0)
		{
			if (endIndex == 0)
				endIndex = m_Size;

			if ((endIndex - beginIndex) > m_Size)
			{
				std::cout << "Error: " << endIndex - beginIndex << " was larger than " << m_Size << std::endl;
				return *this;
			}

			HLStringBase str = SelectDataSource(m_Size);
			return Assign(str, endIndex, beginIndex);
		}

		HLAPI const HLStringBase Substr(uint32 beginIndex, uint32 endIndex = 0) const
		{
			if (endIndex == 0)
				endIndex = m_Size;

			if ((endIndex - beginIndex) > m_Size)
				return *this;

			return HLStringBase(*this, beginIndex, endIndex);
		}

		HLAPI const HLStringBase &ToLowerCase()
		{
			StringType *source = SelectDataSource(m_Size);

			if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
			{
				for (uint32 i = 0; i < m_Size; ++i)
				{
					if (source[i] >= 'A' && source[i] <= 'Z')
						m_StaticData[i] = source[i] - ('A' - 'a');
				}
			}
			else
			{
				for (uint32 i = 0; i < m_Size; ++i)
				{
					if (source[i] >= 'A' && source[i] <= 'Z')
						m_Data[i] = source[i] - ('A' - 'a');
				}
			}

			return *this;
		}

		HLAPI const HLStringBase &ToUpperCase()
		{
			StringType *source = SelectDataSource(m_Size);

			if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
			{
				for (uint32 i = 0; i < m_Size; ++i)
				{
					if (source[i] >= 'a' && source[i] <= 'z')
						m_StaticData[i] = source[i] + ('A' - 'a');
				}
			}
			else
			{
				for (uint32 i = 0; i < m_Size; ++i)
				{
					if (source[i] >= 'a' && source[i] <= 'z')
						m_Data[i] = source[i] + ('A' - 'a');
				}
			}

			return *this;
		}

		HLAPI uint32 CountOf(StringType letter) const
		{
			uint32 count = 0;
			const StringType *source = C_Str();

			for (uint32 i = 0; i < m_Size; ++i)
			{
				if (source[i] == letter)
					++count;
			}

			return count;
		}

		HLAPI uint64 Hash() const
		{
			uint32 i;
			uint64 hash = 2166136261UL;
			const StringType *source = C_Str();
			unsigned char *p = (unsigned char*)source;

			for (i = 0; i < m_Size; i++)
				hash = (hash ^ p[i]) * 16777619;

			return hash;
		}

		HLAPI bool IsEmpty() const
		{
			return m_Size == 0;
		}

		HLAPI bool Contains(const StringType letter, uint32 offset = 0) const
		{
			bool hasLetter = false;
			const StringType *source = C_Str();

			if (IsEmpty())
				return false;

			for (uint32 i = offset; i < m_Size; ++i)
			{
				if (source[i] == letter)
				{
					hasLetter = true;
					break;
				}
			}

			return hasLetter;
		}

		HLAPI bool Contains(const HLStringBase &other, uint32 offset = 0) const
			{
			uint32 i = offset;
			uint32 j = 0;
			const StringType *source = C_Str();

			if (IsEmpty())
				return false;

			while (source[i] != '\0')
			{
				if (source[i] == other[j])
				{
					while (source[i] == other[j] && other[j] != '\0')
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

		HLAPI bool StartsWith(const StringType letter) const
		{
			const StringType *source = C_Str();

			if (source[0] == letter)
				return true;

			return false;
		}

		HLAPI bool StartsWith(const HLStringBase &other) const
		{
			const StringType *source = C_Str();
			uint32 equalCount = 0;

			for (uint32 i = 0; i < other.m_Size; ++i)
			{
				if (source[i] == other[i])
					++equalCount;
			}

			if (equalCount == other.m_Size)
				return true;

			return false;
		}

		HLAPI bool EndsWith(const StringType letter) const
		{
			if (IsEmpty())
				return false;

			const StringType *source = C_Str();

			if (source[m_Size - 1] == letter)
				return true;

			return false;
		}

		HLAPI bool EndsWith(const HLStringBase &other) const
		{
			uint32 equalCount = 0;
			uint32 start_pos = m_Size - other.m_Size;
			const StringType *source = C_Str();

			for (uint32 i = start_pos; i < m_Size; ++i)
			{
				for (uint32 j = 0; j < other.m_Size; ++j)
				{
					if (source[i] == other[j])
						++equalCount;
				}
			}

			if (equalCount == other.m_Size)
				return true;

			return false;
		}

		HLAPI int32 Compare(const HLStringBase &other) const
		{
			uint32 lhsSize = Length();
			uint32 rhsSize = other.Length();
			const StringType *source = C_Str();

			int32 result = utils::Compare<StringType>(source, other.C_Str(), lhsSize <= rhsSize ? lhsSize : rhsSize);

			if (result != 0)
				return result;

			if (lhsSize < rhsSize)
				return -1;

			if (lhsSize > rhsSize)
				return 1;

			return 0;
		}

		HLAPI int32 Compare(uint32 pos1, uint32 count1, const HLStringBase &other, uint32 pos2 = 0, uint32 count2 = NPOS) const
		{
			if (count2 == NPOS)
				count2 = other.Length();

			StringType *source = SelectDataSource(m_Size);

			uint32 lhsSize = pos1 + count1;
			uint32 rhsSize = pos2 + count2;

			uint32 newLhsSize;
			uint32 newRhsSize;
			int32 result = utils::Compare<StringType>(source, pos1, lhsSize, other.C_Str(), pos2, rhsSize, &newLhsSize, &newRhsSize);

			if (result != 0)
				return result;

			if (newLhsSize < newRhsSize)
				return -1;

			if (newLhsSize > newRhsSize)
				return 1;

			return 0;
		}

		HLAPI static HLString FromWideString(const wchar_t *str)
		{
			std::wstring ws(str);
			std::string s(ws.begin(), ws.end());
			return s.c_str();
		}

		HLAPI static HLStringWide FromCString(const char *str)
		{
			std::string s(str);
			std::wstring ws(s.begin(), s.end());
			return ws.c_str();
		}

		template<typename IteratorType1, typename IteratorType2>
		HLAPI static bool LexicographicalCompare(IteratorType1 first1, IteratorType1 last1, IteratorType2 first2, IteratorType2 last2)
		{
			for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
			{
				if (*first1 < *first2) return true;
				if (*first2 < *first1) return false;
			}

			return (first1 == last1) && (first2 != last2);
		}

		HLAPI StringType *operator*()
		{
			return SelectDataSource(m_Size);
		}

		HLAPI const StringType *operator*() const
		{
			if (m_Size < HL_STRING_MAX_STACK_STRING_SIZE)
				return m_StaticData;

			return m_Data;
		}

		HLAPI operator StringType*()
		{
			return SelectDataSource(m_Size);
		}

		HLAPI operator const StringType*() const
		{
			return C_Str();
		}

		HLAPI bool operator==(const StringType *other) const
		{
			return Compare(HLStringBase(other)) == 0;
		}

		HLAPI bool operator==(const HLStringBase &other) const
		{
			return Compare(other) == 0;
		}

		HLAPI bool operator!=(const StringType *other) const
		{
			return !(*this == other);
		}

		HLAPI bool operator!=(const HLStringBase &other) const
		{
			return !(*this == other);
		}

		HLAPI bool operator<(const StringType *other) const
		{
			return Compare(HLStringBase(other)) < 0;
		}

		HLAPI bool operator<(const HLStringBase &other) const
		{
			return Compare(other) < 0;
		}

		HLAPI bool operator>(const StringType *other) const
		{
			return Compare(HLStringBase(other)) > 0;
		}

		HLAPI bool operator>(const HLStringBase &other) const
		{
			return Compare(other) > 0;
		}

		HLAPI bool operator<=(const StringType *other) const
		{
			return Compare(HLStringBase(other)) <= 0;
		}

		HLAPI bool operator<=(const HLStringBase &other) const
		{
			return Compare(other) <= 0;
		}

		HLAPI bool operator>=(const StringType *other) const
		{
			return Compare(HLStringBase(other)) >= 0;
		}

		HLAPI bool operator>=(const HLStringBase &other) const
		{
			return Compare(other) >= 0;
		}

		HLAPI friend HLStringBase operator-(HLStringBase str, const HLStringBase &other)
		{
			return str.Remove(other);
		}

		HLAPI friend HLStringBase operator-(HLStringBase str, const StringType letter)
		{
			return str.Remove(letter);
		}

		HLAPI friend HLStringBase operator-(HLStringBase str, const StringType *other)
		{
			return str.Remove(other);
		}

		HLAPI friend HLStringBase operator+(HLStringBase str, const HLStringBase &other)
		{
			return str.Append(other);
		}

		HLAPI friend HLStringBase operator+(HLStringBase str, const StringType letter)
		{
			return str.Append(letter);
		}

		HLAPI friend HLStringBase operator+(HLStringBase str, const StringType *other)
		{
			return str.Append(other);
		}

		HLAPI HLStringBase &operator-=(const HLStringBase &other)
		{
			return Remove(other);
		}

		HLAPI HLStringBase &operator-=(const StringType letter)
		{
			return Remove(letter);
		}

		HLAPI HLStringBase &operator+=(const HLStringBase &other)
		{
			return Append(other);
		}

		HLAPI HLStringBase &operator+=(const StringType letter)
		{
			return Append(letter);
		}

		HLAPI StringType &operator[](uint32 index)
		{
			HL_ASSERT(index <= m_Size);
			StringType *source = SelectDataSource(m_Size);
			return source[index];
		}

		HLAPI const StringType &operator[](uint32 index) const
		{
			HL_ASSERT(index <= m_Size);
			const StringType *source = C_Str();
			return source[index];
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLStringBase &str)
		{
			const StringType *source = str.C_Str();

			for (uint32 i = 0; i < str.m_Size; ++i)
				stream << source[i];

			return stream;
		}

		template<typename T>
		HLAPI HL_FORCE_INLINE static HLStringBase ToString(const T &value)
		{
			std::stringstream ss;
			ss << value;
			return HLStringBase(ss.str().c_str());
		}

		template<typename T>
		HLAPI HL_FORCE_INLINE friend HLStringBase &operator<<(HLStringBase &str, const T &value)
		{
			HLStringBase newStr = HLStringBase::ToString(value);
			return str.Append(newStr);
		}

		template<typename T>
		HLAPI HL_FORCE_INLINE friend HLStringBase &operator>>(HLStringBase &str, const T &value)
		{
			HLStringBase newStr = HLStringBase::ToString(value);
			return str.Remove(newStr);
		}

		HLAPI int32 ToInt32() const
		{
			const StringType *source = C_Str();
			return atoi(source);
		}

		HLAPI uint32 ToUInt32() const
		{
			const StringType *source = C_Str();
			return (uint32)atoi(source);
		}

		HLAPI bool ToBool() const
		{
			const StringType *source = C_Str();
			return strcmp(source, "1") == 0 || strcmp(source, "true") == 0 || strcmp(source, "TRUE") == 0;
		}

		HLString ToUTF8() const
		{
			return utils::ToUTF8(*this);
		}

		HLString16 ToUTF16() const
		{
			return utils::ToUTF16(*this);
		}

		HLString32 ToUTF32() const
		{
			return utils::ToUTF32(*this);
		}
	};
}

namespace std
{
	template<>
	struct hash<highlo::HLString>
	{
		std::size_t operator()(const highlo::HLString &str) const
		{
			return hash<uint64>()(str.Hash());
		}
	};

	template<>
	struct hash<highlo::HLString16>
	{
		std::size_t operator()(const highlo::HLString16 &str) const
		{
			return hash<uint64>()(str.Hash());
		}
	};

	template<>
	struct hash<highlo::HLString32>
	{
		std::size_t operator()(const highlo::HLString32 &str) const
		{
			return hash<uint64>()(str.Hash());
		}
	};
}

