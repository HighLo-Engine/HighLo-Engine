#pragma once

#include "engine/core/HLCore.h"
#include "engine/core/Log.h"

namespace highlo
{
	class HLString
	{
	public:

		static const uint32 NPOS = static_cast<uint32>(-1);

		HLAPI HLString() = default;
		HLAPI HLString(const char *data);
		HLAPI HLString(const wchar_t *data);
		HLAPI HLString(const std::string &str);
		HLAPI HLString(const std::wstring &wideStr);
		HLAPI HLString(const HLString &other);
		HLAPI HLString(const HLString &other, uint32 length);
		HLAPI HLString(const HLString &other, uint32 start, uint32 end);
		HLAPI HLString(HLString &&other) noexcept;
		HLAPI ~HLString();

		HLAPI HLString &operator=(const HLString &other);
		HLAPI HLString &operator=(HLString &&other) noexcept;

		HLAPI void Clear();
		HLAPI void Resize(int64 size);
		HLAPI uint32 Length() const;

		HLAPI wchar_t *W_Str();
		HLAPI const wchar_t *W_Str() const;

		HLAPI char *C_Str();
		HLAPI const char *C_Str() const;

		HLAPI char At(uint32 index);
		HLAPI const char At(uint32 index) const;

		HLAPI HLString &Append(const char letter);
		HLAPI HLString &Append(const HLString &other);
		HLAPI HLString &Remove(const char letter);
		HLAPI HLString &Remove(const HLString &other);

		HLAPI uint32 FirstIndexOf(const char letter, uint32 offset = 0) const;
		HLAPI uint32 FirstIndexOf(const HLString &other, uint32 offset = 0) const;
		HLAPI uint32 IndexOf(const char letter, uint32 offset = 0) const;
		HLAPI uint32 IndexOf(const HLString &other, uint32 offset = 0) const;
		HLAPI uint32 LastIndexOf(const char letter, uint32 offset = 0) const;
		HLAPI uint32 LastIndexOf(const HLString &other, uint32 offset = 0) const;
		HLAPI uint32 FirstIndexNotOf(const char letter, uint32 offset = 0) const;
		HLAPI uint32 FirstIndexNotOf(const HLString &other, uint32 offset = 0) const;

		HLAPI std::vector<HLString> Split(char delimiter);
		HLAPI HLString *Split(char delimiter, uint32 *outWordCount);
		HLAPI HLString &Replace(const HLString &find, const HLString &replaceValue, uint32 occurencesToReplace = 0);
		HLAPI HLString &Reverse();
		HLAPI HLString Substr(uint32 beginIndex, uint32 endIndex = 0) const;

		HLAPI const HLString &ToLower() const;
		HLAPI const HLString &ToUpper() const;

		HLAPI void Print() const;

		HLAPI bool IsEmpty() const;
		HLAPI bool Contains(const char letter, uint32 offset = 0) const;
		HLAPI bool Contains(const HLString &other, uint32 offset = 0) const;
		HLAPI bool StartsWith(const char letter) const;
		HLAPI bool StartsWith(const HLString &other) const;
		HLAPI bool EndsWith(const char letter) const;
		HLAPI bool EndsWith(const HLString &other) const;

		HLAPI char *operator*();
		HLAPI const char *operator*() const;

		HLAPI operator char *();
		HLAPI operator const char *() const;

		HLAPI bool operator==(const char *other) const;
		HLAPI bool operator==(const HLString &other) const;
		HLAPI bool operator!=(const char *other) const;
		HLAPI bool operator!=(const HLString &other) const;
		HLAPI bool operator<(const char *other) const;
		HLAPI bool operator<(const HLString &other) const;
		HLAPI bool operator>(const char *other) const;
		HLAPI bool operator>(const HLString &other) const;
		HLAPI bool operator<=(const char *other) const;
		HLAPI bool operator<=(const HLString &other) const;
		HLAPI bool operator>=(const char *other) const;
		HLAPI bool operator>=(const HLString &other) const;

		HLAPI friend HLString operator-(HLString str, const HLString &other);
		HLAPI friend HLString operator-(HLString str, const char letter);
		HLAPI friend HLString operator-(HLString str, const char *other);
		HLAPI friend HLString operator+(HLString str, const HLString &other);
		HLAPI friend HLString operator+(HLString str, const char letter);
		HLAPI friend HLString operator+(HLString str, const char *other);
		HLAPI HLString &operator-=(const HLString &other);
		HLAPI HLString &operator-=(const char letter);
		HLAPI HLString &operator+=(const HLString &other);
		HLAPI HLString &operator+=(const char letter);

		HLAPI char &operator[](uint32 index);
		HLAPI const char &operator[](uint32 index) const;

		HLAPI friend std::ostream &operator<<(std::ostream &stream, const HLString &str);

		template<typename T>
		HLAPI FORCEINLINE static HLString ToString(const T &value)
		{
			std::stringstream ss;
			ss << value;
			return HLString(ss.str());
		}

		template<typename T>
		HLAPI FORCEINLINE friend HLString &operator<<(HLString &str, const T &value)
		{
			HLString newStr = HLString::ToString(value);
			return str.Append(newStr);
		}

		template<typename T>
		HLAPI FORCEINLINE friend HLString &operator>>(HLString &str, const T &value)
		{
			HLString newStr = HLString::ToString(value);
			return str.Remove(newStr);
		}

	private:
		char *m_Data = 0;
		uint32 m_Size = 0;
	};
}