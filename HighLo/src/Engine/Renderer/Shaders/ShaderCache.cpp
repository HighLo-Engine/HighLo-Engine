#include "HighLoPch.h"
#include "ShaderCache.h"

#include "Engine/Application/Application.h"

namespace highlo
{
	bool ShaderCache::HasChanged(const FileSystemPath &filePath, const HLString &source)
	{
		HLString shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath.String();
		std::map<HLString, uint64> shaderCache;
		Deserialize(shaderCache);

		uint64 hash = source.Hash();
		if (shaderCache.find(filePath.String()) == shaderCache.end() || shaderCache.at(filePath.String()) != hash)
		{
			shaderCache[filePath.String()] = hash;
			Serialize(shaderCache);
			return true;
		}

		return false;
	}

	void ShaderCache::Serialize(const std::map<HLString, uint64> &shaderCache)
	{
		// TODO: Write into shaderRegistryPath
		HLString shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath.String();
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		// TODO: Load Config from shaderRegistryPath
		HLString shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath.String();
	}
}

