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