// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2022-01-13) Added LexicalCast function (it can convert any type to any different type)
//     - 1.3 (2022-01-13) Added String conversion functions
//     - 1.2 (2021-11-19) Added Compare functions
//     - 1.1 (2021-09-29) Removed String conversion function because it should be natively now possible with our own String class
//     - 1.0 (2021-09-26) initial release
//

#pragma once

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
		bool NextLine(int32 index, const std::vector<int32> &lines);
		HLString FillWithLeading(HLString &str, const HLString &leadingCharacter, uint32 count = 1);

		template<typename ToType, typename FromType>
		static ToType LexicalCast(FromType const &x)
		{
			std::stringstream os;
			ToType result;

			os << x;
			os >> result;

			return result;
		}

		template<typename StringType>
		static StringType *CopySubStr(const StringType *str, uint32 pos, uint32 size)
		{
			HL_ASSERT(pos < size);

			StringType *result = new StringType[size + 1];
			result[size] = '\0';
			memcpy(result, str + pos, size);

			return result;
		}

		template<typename StringType>
		static int32 Compare(const StringType *str1, const StringType *str2, uint32 size)
		{
			return memcmp((const void *)str1, (const void *)str2, (size_t)size);
		}

		template<typename StringType>
		static int32 Compare(const StringType *str1, uint32 pos1, uint32 size1, const StringType *str2, uint32 pos2, uint32 size2, uint32 *outLhsSize, uint32 *outRhsSize)
		{
			StringType *str1Copied = utils::CopySubStr<StringType>(str1, pos1, size1);
			StringType *str2Copied = utils::CopySubStr<StringType>(str2, pos2, size2);

			uint32 str1Len = utils::GetSizeOfUnknownStringType<StringType>(str1Copied);
			uint32 str2Len = utils::GetSizeOfUnknownStringType<StringType>(str2Copied);
			int32 result = utils::Compare<StringType>(str1Copied, str2Copied, str1Len <= str2Len ? str1Len : str2Len);

			if (outLhsSize)
				*outLhsSize = str1Len;

			if (outRhsSize)
				*outRhsSize = str2Len;

			delete[] str1Copied;
			delete[] str2Copied;

			return result;
		}
	}
}

