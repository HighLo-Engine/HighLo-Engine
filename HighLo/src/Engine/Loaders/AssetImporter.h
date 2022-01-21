// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-21) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"

namespace highlo
{
	class AssetImporter
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static bool TryLoadData(const AssetMetaData &assetInfo, Ref<Asset> &asset);

		HLAPI static void Serialize(const AssetMetaData &assetInfo, const Ref<Asset> &asset);
		HLAPI static void Serialize(const Ref<Asset> &asset);

	private:


	};
}

