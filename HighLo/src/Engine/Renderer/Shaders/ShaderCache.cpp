// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderCache.h"

#include "Engine/Application/Application.h"
#include "Engine/Loaders/DocumentWriter.h"

namespace highlo
{
	static std::map<HLString, uint64> s_ShaderCache;

	void ShaderCache::Init()
	{
		Deserialize(s_ShaderCache);
	}
	
	void ShaderCache::Shutdown()
	{
		Serialize(s_ShaderCache);
	}

	bool ShaderCache::HasChanged(const FileSystemPath &filePath, const HLString &source)
	{
		uint64 hash = source.Hash();

		if (s_ShaderCache.find(filePath.String()) == s_ShaderCache.end() || s_ShaderCache.at(filePath.String()) != hash)
		{
			s_ShaderCache[filePath.String()] = hash;
			return true;
		}

		return false;
	}

	void ShaderCache::Serialize(const std::map<HLString, uint64> &shaderCache)
	{
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath;
		Ref<DocumentWriter> writer = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);

		writer->BeginArray();
		for (auto &[filePath, hash] : shaderCache)
		{
			writer->BeginObject();
			writer->WriteUInt64(filePath, hash);
			writer->EndObject();
		}
		writer->EndArray();

		bool success = writer->WriteOut();
		HL_ASSERT(success);
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath;
		Ref<DocumentWriter> reader = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);
		bool readSuccess = reader->ReadContents();
		bool success = reader->ReadUInt64ArrayMap("", shaderCache);

	}
}

