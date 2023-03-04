// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Serializable.h"

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	class Serializer : public IsSharedReference
	{
	public:

		HLAPI static void Serialize(const FileSystemPath &filePath);
		HLAPI static void Deserialize(const FileSystemPath &filePath);

		HLAPI static void RegisterSerializable(const Ref<Serializable> &obj);
		HLAPI static void UnregisterSerializable(const Ref<Serializable> &obj);
	};
}

