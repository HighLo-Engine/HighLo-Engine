// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ShaderCache.h"

#include "Engine/Application/Application.h"
#include "Engine/Loaders/DocumentWriter.h"
#include "Engine/Loaders/DocumentReader.h"

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

	//	writer->BeginArray();
	//	for (auto &[filePath, hash] : shaderCache)
	//	{
	//		writer->BeginObject();
	//		writer->WriteUInt64(filePath, hash);
	//		writer->EndObject();
	//	}
	//	writer->EndArray();

	//	if (writer->WriteUInt64ArrayMap("shaderCache", shaderCache))
	//	{
	//		bool success = writer->WriteOut();
	//		HL_ASSERT(success);
	//	}
	//	else
	//	{
	//		HL_CORE_ERROR("Could not write array map!");
	//	}

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
	//	Ref<DocumentReader> reader = DocumentReader::Create(shaderRegistryPath, DocumentType::Json);
		Ref<DocumentWriter> reader = DocumentWriter::Create(shaderRegistryPath, DocumentType::Json);


	//	if (reader->ReadContents())
	//	{
	//		if (!reader->ReadUInt64ArrayMap("shaderCache", shaderCache))
	//		{
	//			HL_CORE_ERROR("Could not read ShaderCache!");
	//			HL_ASSERT(false);
	//		}
	//	}
	//	else
	//	{
	//		HL_CORE_ERROR("Could not read file contents!");
	//	}

		bool readSuccess = reader->ReadContents();
		bool success = reader->ReadUInt64ArrayMap("", shaderCache);
	//	HL_ASSERT(success);
	}
}

