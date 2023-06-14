// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-21) initial release
//

#pragma once

#include "Asset.h"
#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	class AssetRegistry
	{
	public:

		HLAPI AssetMetaData &operator[](const FileSystemPath &path);
		HLAPI const AssetMetaData &Get(const FileSystemPath &path) const;

		HLAPI uint64 Count() const { return m_AssetRegistry.size(); }
		HLAPI bool Contains(const FileSystemPath &path) const;
		HLAPI uint64 Remove(const FileSystemPath &path);
		HLAPI void Clear();

		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::iterator begin() { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::iterator end() { return m_AssetRegistry.end(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::const_iterator cbegin() const { return m_AssetRegistry.begin(); }
		HLAPI std::unordered_map<FileSystemPath, AssetMetaData>::const_iterator cend() const { return m_AssetRegistry.end(); }

		HLAPI FileSystemPath GetKey(const FileSystemPath &path) const;

	private:

		std::unordered_map<FileSystemPath, AssetMetaData> m_AssetRegistry;
	};
}

