// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#define HL_SERIALIZE(name)
#define HL_SERIALIZE_MEMBER(name, member)

#define HL_DESERIALIZE(name)
#define HL_DESERIALIZE_MEMBER(name, member)

// Helpers for bit fields
#define HL_SERIALIZE_BIT(name)
#define HL_SERIALIZE_BIT_MEMBER(name, member)

#define HL_DESERIALIZE_BIT(name)
#define HL_DESERIALIZE_BIT_MEMBER(name, member)

namespace highlo
{
	class Serializable
	{
	public:

		HLAPI virtual void Serialize() = 0;
		HLAPI virtual void Deserialize() = 0;

	};
}

