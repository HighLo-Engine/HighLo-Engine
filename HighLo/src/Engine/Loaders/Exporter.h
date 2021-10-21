// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-27) initial release
//

#pragma once

#include "Engine/Core/SharedReference.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class Exporter
	{
	public:

		HLAPI virtual bool Save(const FileSystemPath &path, const Ref<Asset> &asset) = 0;
	};
}

