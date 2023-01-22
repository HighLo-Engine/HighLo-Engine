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

namespace highlo
{
	class Serializer;

	class Serializable : public IsSharedReference
	{
	public:

		HLAPI Serializable();
		HLAPI virtual ~Serializable();

		HLAPI virtual void Serialize(const Ref<Serializer> &serializer) = 0;
		HLAPI virtual void Deserialize(const Ref<Serializer> &serializer) = 0;

	};
}

