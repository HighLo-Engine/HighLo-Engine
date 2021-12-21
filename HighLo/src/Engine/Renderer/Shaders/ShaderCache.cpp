// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderCache.h"

#include "Engine/Application/Application.h"
#include "Engine/Loaders/DocumentWriter.h"

namespace highlo
{
	std::map<HLString, uint64> ShaderCache::m_Cache;

	void ShaderCache::Init()
	{
		Deserialize(m_Cache);
	}
	
	void ShaderCache::Shutdown()
	{
		Serialize(m_Cache);
	}

	bool ShaderCache::HasChanged(const FileSystemPath &filePath, const HLString &source)
	{
		HLString shaderRegistryPath = HLApplication::Get().GetApplicationSettings().CacheRegistryPath.String();

		uint64 hash = source.Hash();
		if (m_Cache.find(filePath.String()) == m_Cache.end() || m_Cache.at(filePath.String()) != hash)
		{
			m_Cache[filePath.String()] = hash;
			return true;
		}

		return false;
	}

	void ShaderCache::Serialize(const std::map<HLString, uint64> &shaderCache)
	{
		/*
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().CacheRegistryPath;
		Ref<DocumentWriter> writer = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);

		std::vector<DocumentEntry> dataToStore;
		for (auto &[filePath, hash] : shaderCache)
		{
			DocumentEntry entry;
			entry.Key = filePath;
			entry.Value = new void*[1];
			entry.Value[0] = &((uint64)hash);
			entry.ElementSize = sizeof(uint64);
			entry.ElementCount = 1;
			entry.Type = "uint64";
			dataToStore.push_back(entry);
		}

		writer->Write("shaderCache", dataToStore);
		writer->WriteOut();
		*/
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		/*
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().CacheRegistryPath;
		Ref<DocumentWriter> reader = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);

		std::vector<DocumentEntry> result = reader->GetAll();

		for (uint32 i = 0; i < result.size(); ++i)
		{
			DocumentEntry entry = result[i];

			if (entry.Type == "uint64")
			{
				HLString path = entry.Key;
				uint64 hash = (uint64)entry.Value[0];

				shaderCache[path] = hash;
			}
		}
		*/
	}
}

