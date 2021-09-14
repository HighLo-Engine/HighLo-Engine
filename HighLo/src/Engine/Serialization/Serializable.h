// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	class Serializable
	{
	public:

		HLAPI virtual void Serialize() = 0;
		HLAPI virtual void Deserialize() = 0;

	};
}
