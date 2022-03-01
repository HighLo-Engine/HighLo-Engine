// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/UUID.h"
#include "Engine/Core/FileSystemPath.h"
#include "AssetTypes.h"

namespace highlo
{
	using AssetHandle = UUID;

	class Asset : public IsSharedReference
	{
	public:

		AssetHandle Handle;
		uint16 Flags = (uint16)AssetFlag::None;

		HLAPI virtual ~Asset() {}

		HLAPI static AssetType GetStaticType() { return AssetType::None; }
		HLAPI virtual AssetType GetAssetType() const { return AssetType::None; }

		HLAPI bool IsValid() const { return ((Flags & (uint16)AssetFlag::Missing) | (Flags & (uint16)AssetFlag::Invalid)) == 0; }

		HLAPI virtual bool operator==(const Asset &other) const
		{
			return Handle == other.Handle;
		}

		HLAPI virtual bool operator!=(const Asset &other) const
		{
			return !(*this == other);
		}

		HLAPI bool IsFlagSet(AssetFlag flag) const { return (uint16)flag & Flags; }
		HLAPI void SetFlag(AssetFlag flag, bool value = true)
		{
			if (value)
				Flags |= (uint16)flag;
			else
				Flags &= ~(uint16)flag;
		}
	};

	struct AssetMetaData
	{
		AssetHandle Handle = 0;
		AssetType Type;

		FileSystemPath FilePath;
		bool IsDataLoaded = false;

		bool IsValid() const { return Handle != 0; }
	};
}

