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
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().CacheRegistryPath;
		Ref<DocumentWriter> writer = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);

		writer->BeginArray();
		for (auto &[filePath, hash] : shaderCache)
		{
			writer->BeginObject();
			writer->Write(filePath, hash);
			writer->EndObject();
		}
		writer->EndArray();

		bool success = writer->WriteOut();
		HL_ASSERT(success);
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().CacheRegistryPath;
		Ref<DocumentWriter> reader = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);
		bool success = reader->ReadContents();
		success = reader->ReadUint64ArrayMap("", m_Cache);
	}
}

