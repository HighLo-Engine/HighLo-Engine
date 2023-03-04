// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

namespace highlo
{
	class Base64
	{
	public:

		HLAPI static bool IsBase64(Byte c);
		HLAPI static HLString Encode(const Byte *buffer, uint32 bufferLength);
		HLAPI static std::vector<Byte> Decode(const HLString &encodedString);
		HLAPI static HLString DecodeToString(const HLString &encodedString);
	};
}

