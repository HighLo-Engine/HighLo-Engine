// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-10-02) Added begin and end functions to be able to use ForEach loops over Strings
//     - 1.2 (2021-10-01) fixed Bug in Contains methods - the engine does not longer crash if the checked string is empty
//     - 1.1 (2021-09-29) Refactored hole class to support multibyte Strings as well
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Defines/BaseTypes.h"

#include <iostream>

#define HASH_LENGTH 20
#define HL_STRING_MAX_STRING_SIZE 16

namespace highlo
{
	template<typename T>
	class HLStringBase;

	using HLString = HLStringBase<char>;
	using HLString16 = HLStringBase<char16_t>;
	using HLString32 = HLStringBase<char32_t>;
	using HLStringWide = HLStringBase<wchar_t>;

	namespace utils
	{
		static char *CopySubStr(const char *str, uint32 pos, uint32 size)
		{
			HL_ASSERT(pos < size);

			char *result = new char[size + 1];
			result[size] = '\0';
			memcpy(result, str + pos, size);

			return result;
		}

		static int32 Compare(const char *str1, const char *str2, uint32 size)
		{
			return memcmp((const void*)str1, (const void*)str2, (size_t)size);
		}

		static int32 Compare(const char *str1, uint32 pos1, uint32 size1, const char *str2, uint32 pos2, uint32 size2, uint32 *outLhsSize, uint32 *outRhsSize)
		{
			char *str1Copied = utils::CopySubStr(str1, pos1, size1);
			char *str2Copied = utils::CopySubStr(str2, pos2, size2);

			uint32 str1Len = (uint32)strlen(str1Copied);
			uint32 str2Len = (uint32)strlen(str2Copied);
			int32 result = utils::Compare(str1Copied, str2Copied, str1Len <= str2Len ? str1Len : str2Len);

			if (outLhsSize)
				*outLhsSize = str1Len;

			if (outRhsSize)
				*outRhsSize = str2Len;

			delete[] str1Copied;
			delete[] str2Copied;

			return result;
		}

		template<typename T>
		static uint32 GetSizeOfUnknownStringType(const T *str)
		{
			HL_ASSERT(false, "unknown string type!");
			return 0;
		}

		template<>
		uint32 GetSizeOfUnknownStringType(const char *str)
		{
			return (uint32)strlen(str);
		}

		template<>
		uint32 GetSizeOfUnknownStringType(const wchar_t *str)
		{
			return (uint32)wcslen(str);
		}
	}

	template<typename StringType>
	class HLStringBase
	{
	public:

		static const uint32 NPOS = static_cast<uint32>(-1);

		HLAPI HLStringBase()
		{
			Assign("", 0);
		}

		HLAPI HLStringBase(const StringType *data)
		{
			uint32 size = utils::GetSizeOfUnknownStringType<StringType>(data);
			Assign(data, size);
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
			m_DataPointer = other.m_DataPointer;
			m_UsingShortStr = other.m_UsingShortStr;

			// TODO: potentially leaking memory here
			other.m_DataPointer = nullptr;
			other.m_UsingShortStr = true; // Restore default
		}

		HLAPI ~HLStringBase()
		{
			if (!m_UsingShortStr)
			{
				LongStringData *data = (LongStringData*)m_DataPointer;
				delete[] data->Data;
				data->Data = nullptr;

				delete data;
				m_DataPointer = nullptr;
				return;
			}

			ShortStringData *data = (ShortStringData*)m_DataPointer;
			delete data;
			m_DataPointer = nullptr;
		}

		HLAPI HLStringBase &Assign(const StringType *str, uint32 size, uint32 startOffset = 0)
		{
			HL_ASSERT(str);
			HL_ASSERT(startOffset <= size);

			uint32 new_size = size - startOffset;

			if (new_size < HL_STRING_MAX_STRING_SIZE)
			{
				ShortStringData *data = new ShortStringData();
				data->Size = new_size;
				data->Data[data->Size] = '\0';

				if (str)
				{
					//memcpy_s(data->Data, HL_STRING_MAX_STRING_SIZE, str + startOffset, new_size);
					memcpy(data->Data, str + startOffset, new_size);
				}

				// Delete old data, if exists
				if (!m_UsingShortStr && m_DataPointer)
				{
					LongStringData *old_data = (LongStringData*)m_DataPointer;
					if (old_data->Data)
					{
						delete[] old_data->Data;
						old_data->Data = nullptr;
					}
				}
				else if (m_UsingShortStr && m_DataPointer)
				{
					ShortStringData *data = (ShortStringData*)m_DataPointer;
					delete data;
				}

				m_DataPointer = data;
				m_UsingShortStr = true;

				return *this;
			}

			LongStringData *data = new LongStringData();
			data->Data = new StringType[new_size + 1];
			data->Size = new_size;
			data->Data[data->Size] = '\0';

			if (str)
			{
				//memcpy_s(data->Data, data->Size, str + startOffset, new_size);
				memcpy(data->Data, str + startOffset, new_size);
			}

			// Delete old data, if exists
			if (!m_UsingShortStr && m_DataPointer)
			{
				LongStringData *old_data = (LongStringData*)m_DataPointer;
				if (old_data)
				{
					if (old_data->Data)
					{
						delete[] old_data->Data;
						old_data->Data = nullptr;
					}

					delete old_data;
					m_DataPointer = nullptr;
				}
			}
			else if (m_UsingShortStr && m_DataPointer)
			{
				ShortStringData *data = (ShortStringData*)m_DataPointer;
				if (data)
				{
					delete data;
					m_DataPointer = nullptr;
				}
			}

			m_DataPointer = data;
			m_UsingShortStr = false;

			return *this;
		}

		HLAPI HLStringBase &operator=(const HLStringBase &other)
		{
			if (this != &other)
			{
				if (other.m_UsingShortStr)
				{
					ShortStringData *data = (ShortStringData*)other.m_DataPointer;
					ShortStringData *new_data = new ShortStringData();
					new_data->Size = data->Size;
					
					new_data->Data[new_data->Size] = '\0';
					memcpy(new_data->Data, data->Data, data->Size);

					m_UsingShortStr = true;
					m_DataPointer = new_data;
				}
				else
				{
					LongStringData *data = (LongStringData*)other.m_DataPointer;
					LongStringData *new_data = new LongStringData();
					new_data->Size = data->Size;
					new_data->Data = new StringType[data->Size + 1];
					
					new_data->Data[new_data->Size] = '\0';
					memcpy(new_data->Data, data->Data, data->Size);
					
					m_UsingShortStr = false;
					m_DataPointer = new_data;
				}
			}

			return *this;
		}

		HLAPI HLStringBase &operator=(HLStringBase &&other) noexcept
		{
			if (this != &other)
			{
				m_DataPointer = other.m_DataPointer;
				m_UsingShortStr = other.m_UsingShortStr;

				other.m_DataPointer = nullptr;
				other.m_UsingShortStr = true;
			}

			return *this;
		}

		HLAPI StringType *begin()
		{
			return SelectStringSource()[0];
		}

		HLAPI StringType *end()
		{
			return SelectStringSource()[SelectStringSize()];
		}

		HLAPI void Clear()
		{
			if (!m_UsingShortStr)
			{
				LongStringData *data = (LongStringData*)m_DataPointer;
				delete[] data->Data;
				data->Data = nullptr;
			}

			delete m_DataPointer;
			m_DataPointer = nullptr;
			m_UsingShortStr = true; // Restore default
		}

		HLAPI void Resize(uint32 size)
		{
			if (!m_DataPointer)
				return;

			Clear();
			if (size < HL_STRING_MAX_STRING_SIZE)
			{
				ShortStringData *data = new ShortStringData;
				data->Size = size;
				data->Data[data->Size] = '\0';
				m_DataPointer = data;

				m_UsingShortStr = true;
				return;
			}

			LongStringData *data = new LongStringData;
			data->Data = new StringType[size];
			data->Size = size;
			data->Data[data->Size] = '\0';
			m_DataPointer = data;
			m_UsingShortStr = false;
		}

		HLAPI uint32 Length() const
		{
			return SelectStringSize();
		}

		HLAPI wchar_t *W_Str()
		{
			wchar_t *result = new wchar_t[SelectStringSize() + 1];
			for (uint32 i = 0; i < SelectStringSize(); ++i)
				result[i] = SelectStringSource()[i];

			result[SelectStringSize()] = L'\0';
			return result;
		}

		HLAPI const wchar_t *W_Str() const
		{
			wchar_t *result = new wchar_t[SelectStringSize() + 1];
			for (uint32 i = 0; i < SelectStringSize(); ++i)
				result[i] = SelectStringSource()[i];

			result[SelectStringSize()] = L'\0';
			return result;
		}

		HLAPI StringType *C_Str()
		{
			return SelectStringSource();
		}

		HLAPI const StringType *C_Str() const
		{
			return SelectStringSource();
		}

		HLAPI StringType At(uint32 index)
		{
			if (index < SelectStringSize())
				return SelectStringSource()[index];

			return (char)NPOS;
		}

		HLAPI const StringType At(uint32 index) const
		{
			if (index < SelectStringSize())
				return SelectStringSource()[index];

			return (char)NPOS;
		}

		HLAPI HLStringBase &Append(const StringType letter)
		{
			StringType *new_data = new StringType[SelectStringSize() + 2]; // +1 for a character and another +1 for the null terminator
			new_data[SelectStringSize()] = letter;
			new_data[SelectStringSize() + 1] = '\0';

			if (SelectStringSource())
			{
				memcpy(new_data, SelectStringSource(), SelectStringSize());
			}

			return Assign(new_data, SelectStringSize());
		}

		HLAPI HLStringBase &Append(const HLStringBase &other)
		{
			uint32 new_size = SelectStringSize() + other.SelectStringSize();
			StringType *new_data = new StringType[new_size + 1]; // +1 for the null terminator
			new_data[new_size] = '\0';

			if (SelectStringSource())
			{
				memcpy(new_data, SelectStringSource(), SelectStringSize()); // copy existing string
			}

			memcpy((StringType*)(new_data + SelectStringSize()), other.SelectStringSource(), other.SelectStringSize()); // copy appended string

			return Assign(new_data, new_size);
		}

		HLAPI HLStringBase &Remove(const StringType letter)
		{
			if (SelectStringSource() && Contains(letter))
			{
				uint32 i = 0;
				uint32 j = 0;
				uint32 start_pos = IndexOf(letter);
				uint32 new_size = SelectStringSize() - 1;
				StringType *new_data = new StringType[new_size + 1];
				new_data[new_size] = '\0';

				if (start_pos > 0)
				{
					memcpy(new_data, SelectStringSource(), start_pos);
					j = start_pos;
				}

				for (i = start_pos + 1; i < SelectStringSize(); ++i)
				{
					new_data[j] = SelectStringSource()[i];
					++j;
				}

				return Assign(new_data, new_size);
			}

			return *this;
		}

		HLAPI HLStringBase &Remove(const HLStringBase &other)
		{
			if (SelectStringSource() && Contains(other))
			{
				uint32 i = 0;
				uint32 j = 0;
				uint32 start_pos = IndexOf(other);
				uint32 new_size = SelectStringSize() - other.Length();
				StringType *new_data = new StringType[new_size + 1];
				new_data[new_size] = '\0';

				// copy part before the part to remove
				if (start_pos > 0)
				{
					memcpy(new_data, SelectStringSource(), start_pos);
					j = start_pos;
				}

				// copy part after the part to remove
				for (i = start_pos + other.Length(); i < SelectStringSize(); ++i)
				{
					new_data[j] = SelectStringSource()[i];
					++j;
				}

				return Assign(new_data, new_size);
			}

			return *this;
		}

		HLAPI uint32 FirstIndexOf(const StringType letter, uint32 offset = 0) const
		{
			for (uint32 i = offset; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == letter)
					return i;
			}

			return NPOS;
		}

		HLAPI uint32 FirstIndexOf(const HLStringBase &other, uint32 offset = 0) const
		{
			uint32 i = offset;
			uint32 j = 0;

			while (SelectStringSource()[i] != '\0')
			{
				if (SelectStringSource()[i] == other[j])
				{
					while (SelectStringSource()[i] == other[j] && other[j] != '\0')
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
			for (uint32 i = offset; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == letter)
					return i;
			}

			return NPOS;
		}

		HLAPI uint32 IndexOf(const HLStringBase &other, uint32 offset = 0) const
		{
			uint32 i = offset;
			uint32 j = 0;

			while (SelectStringSource()[i] != '\0')
			{
				if (SelectStringSource()[i] == other[j])
				{
					while (SelectStringSource()[i] == other[j] && other[j] != '\0')
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
			HL_ASSERT(offset >= 0 && offset < SelectStringSize(), "Offset is out of bounds!");

			if (offset == 0)
				offset = SelectStringSize();

			// Count how many times the letter exists
			uint32 letterCount = 0;
			for (uint32 i = 0; i < offset; ++i)
			{
				if (SelectStringSource()[i] == letter)
					++letterCount;
			}

			// If No Letter exists, return
			if (0 == letterCount)
				return NPOS;

			// Get the last letter index
			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (letterCount == 1 && SelectStringSource()[i] == letter)
					return i;

				if (SelectStringSource()[i] == letter && letterCount > 0)
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
				uint32 i = SelectStringSize() - other.Length();
				uint32 j = 0;

				while (SelectStringSource()[i] != '\0')
				{
					if (SelectStringSource()[i] == other[j])
					{
						while (SelectStringSource()[i] == other[j] && other[j] != '\0')
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
			const int32 len = static_cast<int32>(strspn(SelectStringSource() + offset, &letter));
			if (len + offset >= SelectStringSize())
				return NPOS;

			return len + offset;
		}

		HLAPI uint32 FirstIndexNotOf(const HLStringBase &other, uint32 offset = 0) const
		{
			const int32 len = static_cast<int32>(strspn(SelectStringSource() + offset, *other));
			if (len + offset >= SelectStringSize())
				return NPOS;

			return len + offset;
		}

		HLAPI std::vector<HLStringBase> Split(StringType delimiter)
		{
			uint32 wordCount = 1;
			uint32 charIdx = 0;
			uint32 wordBeginIdx = 0;
			HLStringBase word;

			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == delimiter)
					++wordCount;
			}

			std::vector<HLStringBase> result(wordCount);
			for (uint32 i = 0; i < wordCount; ++i)
			{
				wordBeginIdx = charIdx;
				while (SelectStringSource()[charIdx] != delimiter && SelectStringSource()[charIdx] != '\0')
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

			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == delimiter)
					++wordCount;
			}

			HLStringBase *result = new HLStringBase[wordCount];
			for (uint32 i = 0; i < wordCount; ++i)
			{
				wordBeginIdx = charIdx;
				while (SelectStringSource()[charIdx] != delimiter && SelectStringSource()[charIdx] != '\0')
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

			StringType *newString = new StringType[SelectStringSize() + 512 + 1]; // +1 for null termination character
			StringType *strValuePointer = &newString[0];
			const StringType *tmp = SelectStringSource();
			uint32 findLength = find.Length();
			uint32 replaceLength = replaceValue.Length();
			uint32 occurencesReplaced = 0;

			newString[SelectStringSize() + 512] = '\0';

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

			return Assign(newString, utils::GetSizeOfUnknownStringType<StringType>(newString));
		}

		HLAPI HLStringBase &Reverse()
		{
			for (uint32 i = 0; i < SelectStringSize() / 2; i++)
			{
				StringType temp = SelectStringSource()[i];
				SelectStringSource()[i] = SelectStringSource()[SelectStringSize() - i - 1];
				SelectStringSource()[SelectStringSize() - i - 1] = temp;
			}

			return *this;
		}

		HLAPI HLStringBase &Substr(uint32 beginIndex, uint32 endIndex = 0)
		{
			if (endIndex == 0)
				endIndex = SelectStringSize();

			if ((endIndex - beginIndex) > SelectStringSize())
				return *this;

			return Assign(SelectStringSource(), endIndex, beginIndex);
		}

		HLAPI HLStringBase &ToLowerCase()
		{
			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] >= 'A' && SelectStringSource()[i] <= 'Z')
					SelectStringSource()[i] = SelectStringSource()[i] - ('A' - 'a');
			}

			return *this;
		}

		HLAPI HLStringBase &ToUpperCase()
		{
			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] >= 'a' && SelectStringSource()[i] <= 'z')
					SelectStringSource()[i] = SelectStringSource()[i] + ('A' - 'a');
			}

			return *this;
		}

		HLAPI uint32 CountOf(StringType letter) const
		{
			uint32 count = 0;
			for (uint32 i = 0; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == letter)
					++count;
			}

			return count;
		}

		HLAPI uint64 Hash() const
		{
			uint32 i;
			uint64 hash = 2166136261UL;
			unsigned char *p = (unsigned char*)SelectStringSource();

			for (i = 0; i < SelectStringSize(); i++)
				hash = (hash ^ p[i]) * 16777619;

			return hash;
		}

		HLAPI bool IsEmpty() const
		{
			return SelectStringSize() == 0;
		}

		HLAPI bool Contains(const StringType letter, uint32 offset = 0) const
		{
			bool hasLetter = false;

			if (IsEmpty())
				return false;

			for (uint32 i = offset; i < SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == letter)
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

			if (IsEmpty())
				return false;

			while (SelectStringSource()[i] != '\0')
			{
				if (SelectStringSource()[i] == other[j])
				{
					while (SelectStringSource()[i] == other[j] && other[j] != '\0')
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
			if (SelectStringSource()[0] == letter)
				return true;

			return false;
		}

		HLAPI bool StartsWith(const HLStringBase &other) const
		{
			uint32 equalCount = 0;
			for (uint32 i = 0; i < other.SelectStringSize(); ++i)
			{
				if (SelectStringSource()[i] == other[i])
					++equalCount;
			}

			if (equalCount == other.SelectStringSize())
				return true;

			return false;
		}

		HLAPI bool EndsWith(const StringType letter) const
		{
			if (IsEmpty())
				return false;

			if (SelectStringSource()[SelectStringSize() - 1] == letter)
				return true;

			return false;
		}

		HLAPI bool EndsWith(const HLStringBase &other) const
		{
			uint32 equalCount = 0;
			uint32 start_pos = SelectStringSize() - other.SelectStringSize();

			for (uint32 i = start_pos; i < SelectStringSize(); ++i)
			{
				for (uint32 j = 0; j < other.SelectStringSize(); ++j)
				{
					if (SelectStringSource()[i] == other[j])
						++equalCount;
				}
			}

			if (equalCount == other.SelectStringSize())
				return true;

			return false;
		}

		HLAPI int32 Compare(const HLStringBase &other) const
		{
			uint32 lhsSize = Length();
			uint32 rhsSize = other.Length();

			int32 result = utils::Compare(SelectStringSource(), other.SelectStringSource(), lhsSize <= rhsSize ? lhsSize : rhsSize);

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

			uint32 lhsSize = pos1 + count1;
			uint32 rhsSize = pos2 + count2;

			uint32 newLhsSize;
			uint32 newRhsSize;
			int32 result = utils::Compare(SelectStringSource(), pos1, lhsSize, other.SelectStringSource(), pos2, rhsSize, &newLhsSize, &newRhsSize);

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
			return SelectStringSource();
		}

		HLAPI const StringType *operator*() const
		{
			return SelectStringSource();
		}

		HLAPI operator StringType*()
		{
			return SelectStringSource();
		}

		HLAPI operator const StringType*() const
		{
			return SelectStringSource();
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
			HL_ASSERT(index <= SelectStringSize());
			return SelectStringSource()[index];
		}

		HLAPI const StringType &operator[](uint32 index) const
		{
			HL_ASSERT(index <= SelectStringSize());
			return SelectStringSource()[index];
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLStringBase &str)
		{
			for (uint32 i = 0; i < str.SelectStringSize(); ++i)
				stream << str.SelectStringSource()[i];

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
			return atoi(SelectStringSource());
		}

		HLAPI uint32 ToUInt32() const
		{
			return (uint32)atoi(SelectStringSource());
		}

		HLAPI bool ToBool() const
		{
			return strcmp(SelectStringSource(), "1") == 0 || strcmp(SelectStringSource(), "true") == 0 || strcmp(SelectStringSource(), "TRUE") == 0;
		}

	protected:

		HL_FORCE_INLINE StringType *SelectStringSource()
		{
			HL_ASSERT(m_DataPointer);

			if (m_UsingShortStr)
			{
				return &(((ShortStringData*)m_DataPointer)->Data)[0];
			}
			else
			{
				return ((LongStringData*)m_DataPointer)->Data;
			}
		}

		HL_FORCE_INLINE const StringType *SelectStringSource() const
		{
			HL_ASSERT(m_DataPointer);

			if (m_UsingShortStr)
			{
				return &(((ShortStringData*)m_DataPointer)->Data)[0];
			}
			else
			{
				return ((LongStringData*)m_DataPointer)->Data;
			}
		}

		HL_FORCE_INLINE uint32 SelectStringSize()
		{
			if (!m_DataPointer)
				return 0;

			if (m_UsingShortStr)
			{
				return ((ShortStringData*)m_DataPointer)->Size;
			}
			else
			{
				return ((LongStringData*)m_DataPointer)->Size;
			}
		}

		HL_FORCE_INLINE uint32 SelectStringSize() const
		{
			if (!m_DataPointer)
				return 0;

			if (m_UsingShortStr)
			{
				return ((ShortStringData*)m_DataPointer)->Size;
			}
			else
			{
				return ((LongStringData*)m_DataPointer)->Size;
			}
		}

		struct LongStringData
		{
			StringType *Data = nullptr;
			uint32 Size = 0;
		};

		struct ShortStringData
		{
			StringType Data[HL_STRING_MAX_STRING_SIZE] = { 0 };
			uint32 Size = 0;
		};

		// Either points to LongStringData or to ShortStringData
		void *m_DataPointer = nullptr;
		bool m_UsingShortStr = true; // Determines which string version is currently used
	};

	class HLStringUTF8 : public HLStringBase<char>
	{
	public:

		HLAPI static uint32 UTF8StringLength(const HLString &str)
		{
			uint32 length = 0;

			for (uint32 i = 0; i < MAX_UINT32; ++i, ++length)
			{
				int32 c = (int32)str[i];
				if (c == 0)
				{
					// Hit the null-termination character
					break;
				}

				if (c >= 0 && c < 127)
				{
					// normal 1-byte character
					// this is not necessary, but just for clarification
					i += 0;
				}
				else if ((c & 0xE0) == 0xC0)
				{
					// 2-byte character
					i += 1;
				}
				else if ((c & 0xF0) == 0xE0)
				{
					// 3-byte character
					i += 2;
				}
				else if ((c & 0xF8) == 0xF0)
				{
					// 4-byte character
					i += 3;
				}
				else
				{
					return 0;
				}
			}

			return length;
		}

		HLAPI static bool FromString(const HLString &text, uint32 offset, int32 *out_codepoint, unsigned char *out_advance)
		{
			HL_ASSERT(offset < text.Length());
			const char *bytes = *text;
			int32 codepoint = (int32)bytes[offset];

			if (codepoint >= 0 && codepoint < 0x7F)
			{
				// Normal single-byte ascii character.
				*out_advance = 1;
				*out_codepoint = codepoint;
				return true;
			}
			else if ((codepoint & 0xE0) == 0xC0)
			{
				// Double-byte character
				codepoint = ((bytes[offset + 0] & 0b00011111) << 6) +
					(bytes[offset + 1] & 0b00111111);
				*out_advance = 2;
				*out_codepoint = codepoint;
				return true;
			}
			else if ((codepoint & 0xF0) == 0xE0)
			{
				// Triple-byte character
				codepoint = ((bytes[offset + 0] & 0b00001111) << 12) +
					((bytes[offset + 1] & 0b00111111) << 6) +
					(bytes[offset + 2] & 0b00111111);
				*out_advance = 3;
				*out_codepoint = codepoint;
				return true;
			}
			else if ((codepoint & 0xF8) == 0xF0)
			{
				// 4-byte character
				codepoint = ((bytes[offset + 0] & 0b00000111) << 18) +
					((bytes[offset + 1] & 0b00111111) << 12) +
					((bytes[offset + 2] & 0b00111111) << 6) +
					(bytes[offset + 3] & 0b00111111);
				*out_advance = 4;
				*out_codepoint = codepoint;
				return true;
			}

			// NOTE: Not supporting 5 and 6-byte characters; return as invalid UTF-8.
			*out_advance = 0;
			*out_codepoint = 0;
			HL_CORE_ERROR("Not supporting 5 and 6-byte characters; Invalid UTF-8.");
			return false;
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

	template<>
	struct hash<highlo::HLStringWide>
	{
		std::size_t operator()(const highlo::HLStringWide &str) const
		{
			return hash<uint64>()(str.Hash());
		}
	};

	template<>
	struct hash<highlo::HLStringUTF8>
	{
		std::size_t operator()(const highlo::HLStringUTF8 &str) const
		{
			return hash<uint64>()(str.Hash());
		}
	};
}

