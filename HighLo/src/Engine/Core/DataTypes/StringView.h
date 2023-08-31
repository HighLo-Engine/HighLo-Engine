// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-22) initial release
//

#pragma once

namespace highlo
{
	template<typename T>
	class HLStringBase;

	template<typename T>
	class HLStringViewBase;

	using HLStringView = HLStringViewBase<char>;
	using HLStringView16 = HLStringViewBase<char16_t>;
	using HLStringView32 = HLStringViewBase<char32_t>;

	template<typename StringType>
	class HLStringViewBase
	{
	public:

		HLAPI HLStringViewBase()
			: m_Data(nullptr), m_Size(0)
		{
		}

		HLAPI HLStringViewBase(const HLStringBase<StringType> &str)
			: m_Data(str.C_Str()), m_Size(str.Length())
		{
		}

		HLAPI HLStringViewBase(const std::string &str)
			: m_Data(str.c_str()), m_Size(str.size())
		{
		}

		HLAPI HLStringViewBase(const std::string_view &strView)
			: m_Data(strView.data()), m_Size(strView.size())
		{
		}

		HLAPI HLStringViewBase(StringType *str)
			: m_Data(str), m_Size(strlen(str))
		{
		}

		HLAPI HLStringViewBase(const HLStringViewBase<StringType> &other)
			: m_Data(other.m_Data), m_Size(other.m_Size)
		{
		}

		HLAPI HLStringViewBase(HLStringViewBase<StringType> &&other)
		{
			m_Data = other.m_Data;
			m_Size = other.m_Size;

			other.m_Data = nullptr;
			other.m_Size = 0;
		}

		HLAPI HLStringViewBase<StringType> &operator=(const HLStringViewBase<StringType> &other)
		{
			if (this != &other)
			{
				m_Data = other.m_Data;
				m_Size = other.m_Size;
			}

			return *this;
		}

		HLAPI HLStringViewBase<StringType> &&operator=(HLStringViewBase<StringType> &&other)
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

		HLAPI bool IsEmpty() const
		{
			return m_Size == 0;
		}

		HLAPI StringType *operator*()
		{
			return m_Data;
		}

		HLAPI const StringType *operator*() const
		{
			return m_Data;
		}

		HLAPI operator StringType*()
		{
			return m_Data;
		}

		HLAPI operator const StringType*() const
		{
			return m_Data;
		}

		HLAPI StringType &operator[](uint32 index)
		{
			HL_ASSERT(index <= m_Size);
			return m_Data[index];
		}

		HLAPI const StringType &operator[](uint32 index) const
		{
			HL_ASSERT(index <= m_Size);
			return m_Data[index];
		}

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLStringViewBase &str)
		{
			for (uint32 i = 0; i < str.m_Size; ++i)
				stream << str.m_Data[i];

			return stream;
		}

		HLAPI HLStringBase<StringType> ToString()
		{
			return HLStringBase<StringType>(m_Data, m_Size);
		}

	private:

		const StringType *m_Data = nullptr;
		uint32 m_Size = 0;
	};
}

