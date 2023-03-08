// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderCache.h"

#include "Engine/Application/Application.h"
#include "Engine/Loaders/DocumentWriter.h"
#include "Engine/Loaders/DocumentReader.h"

#define SHADER_CACHE_LOG_PREFIX "ShaderCache>  "

namespace highlo
{
	static std::map<HLString, uint64> s_ShaderCache;

	namespace utils
	{
		static void PrintShaderCache()
		{
			for (auto &[name, hash] : s_ShaderCache)
			{
				HL_CORE_TRACE("Name: {0}, Hash: {1}", *name, hash);
			}
		}
	}

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

		writer->WriteUInt64ArrayMap("shaderCache", shaderCache);

		bool writeSuccess = writer->WriteOut();
		HL_ASSERT(writeSuccess);
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath;
		Ref<DocumentReader> reader = DocumentReader::Create(shaderRegistryPath, DocumentType::Json);

		bool readSuccess = reader->ReadContents();
		if (readSuccess)
		{
			if (!reader->ReadUInt64ArrayMap("shaderCache", shaderCache))
			{
				HL_CORE_ERROR(SHADER_CACHE_LOG_PREFIX "[-] Error: Could not read shader cache! [-]");
			}
		}
	}
}

