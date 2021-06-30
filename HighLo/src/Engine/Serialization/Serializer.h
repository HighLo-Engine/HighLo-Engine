#pragma once

#include "Serializable.h"

namespace highlo
{
	class Serializer
	{
	public:

		HLAPI static void Serialize(const HLString &path, Serializable &object);
		HLAPI static void Deserialize(const HLString &path, Serializable &object);
	};
}