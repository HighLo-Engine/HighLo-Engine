// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "AssetRegistry.h"

#include "Engine/Scene/Project.h"

#define SHOULD_SHOW_EVENTS 0
#define ASSET_REGISTRY_LOG_PREFIX "ARegistry>    "

namespace highlo
{
	FileSystemPath AssetRegistry::GetKey(const FileSystemPath &path) const
	{
		auto key = path.RelativePath(Project::GetAssetDirectory());
		if (key.String().IsEmpty())
			key = path.LexicallyNormal();

		return key;
	}

	AssetMetaData &AssetRegistry::operator[](const FileSystemPath &path)
	{
		FileSystemPath key = GetKey(path);

	#if SHOULD_SHOW_EVENTS
		HL_CORE_INFO(ASSET_REGISTRY_LOG_PREFIX "[+] Retrieving Asset '{0}' with key '{1}' [+]", **path, **key);
	#endif // SHOULD_SHOW_EVENTS

		HL_ASSERT(!key.String().IsEmpty());
		return m_AssetRegistry[key];
	}

	const AssetMetaData &AssetRegistry::Get(const FileSystemPath &path) const
	{
		const auto key = GetKey(path);

	#if SHOULD_SHOW_EVENTS
		HL_CORE_INFO(ASSET_REGISTRY_LOG_PREFIX "[+] Retrieving Asset '{0}' with key '{1}' [+]", **path, **key);
	#endif // SHOULD_SHOW_EVENTS
		
		HL_ASSERT(!key.String().IsEmpty());
		return m_AssetRegistry.at(key);
	}

	bool AssetRegistry::Contains(const FileSystemPath &path) const
	{
		auto key = GetKey(path);
		return m_AssetRegistry.find(key) != m_AssetRegistry.end();
	}

	uint64 AssetRegistry::Remove(const FileSystemPath &path)
	{
		auto key = GetKey(path);

	#if SHOULD_SHOW_EVENTS
		HL_CORE_INFO(ASSET_REGISTRY_LOG_PREFIX "[+] Removing Asset '{0}' [+]", **path);
	#endif // SHOULD_SHOW_EVENTS
		
		return m_AssetRegistry.erase(key);
	}

	void AssetRegistry::Clear()
	{
	#if SHOULD_SHOW_EVENTS
		HL_CORE_INFO(ASSET_REGISTRY_LOG_PREFIX "[+] Removing all assets [+]");
	#endif // SHOULD_SHOW_EVENTS

		m_AssetRegistry.clear();
	}
}

