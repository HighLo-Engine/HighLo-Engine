// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Serializable.h"

namespace highlo
{
	class Serializer
	{
	public:

		HLAPI static void Serialize(const HLString &path, const Serializable &object);
		HLAPI static void Deserialize(const HLString &path, Serializable &object);
	};
}

