// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Core/SharedReference.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class Importer
	{
	public:

		HLAPI virtual bool Load(const AssetMetaData &metaData, Ref<Asset> &asset) = 0;
	};
}

