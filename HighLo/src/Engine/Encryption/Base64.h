#pragma once

namespace highlo
{
	class Base64
	{
	public:

		static bool IsBase64(Byte c);
		static HLString Encode(const Byte *buffer, uint32 bufferLength);
		static std::vector<Byte> Decode(const HLString &encodedString);
		static HLString DecodeToString(const HLString &encodedString);
	};
}

