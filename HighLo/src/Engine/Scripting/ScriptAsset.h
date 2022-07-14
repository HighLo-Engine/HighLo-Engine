// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-14) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"

namespace highlo
{
	class ScriptAsset : public Asset
	{
	public:

		HLAPI ScriptAsset() = default;
		HLAPI ScriptAsset(uint32 classID);

		HLAPI uint32 GetClassID() const { return m_ClassID; }

		HLAPI static AssetType GetStaticType() { return AssetType::Script; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		uint32 m_ClassID = 0;
	};
}

