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

		writer->WriteUInt64ArrayMap("shaderCache", shaderCache);

		// CHECK - Example #1
	//	writer->WriteString("Hello", "World");
	//	writer->WriteUInt64("number", 42);

		// CHECK - Example #2
	//	writer->BeginObject();
	//	writer->WriteString("Hello", "World");
	//	writer->WriteUInt64("number", 42);
	//	writer->EndObject();

		// CHECK - Example #3
	//	writer->BeginArray();
	//	for (uint32 i = 0; i < 10; ++i)
	//	{
	//		writer->WriteUInt32("i_" + HLString::ToString(i), i);
	//		writer->WriteString("Hello", "World");
	//	}
	//	writer->EndArray();

		// CHECK - Example #4
	//	writer->BeginArray();
	//	for (uint32 i = 0; i < 10; ++i)
	//	{
	//		writer->BeginObject();
	//		writer->WriteUInt32("i_" + HLString::ToString(i), i);
	//	//	writer->WriteString("Hello", "World");
	//		writer->EndObject();
	//	}
	//	writer->EndArray();

		bool success = writer->WriteOut();
		HL_ASSERT(success);
	}
	
	void ShaderCache::Deserialize(std::map<HLString, uint64> &shaderCache)
	{
		FileSystemPath shaderRegistryPath = HLApplication::Get().GetApplicationSettings().ShaderRegistryPath;
		Ref<DocumentReader> reader = DocumentReader::Create(shaderRegistryPath, DocumentType::Json);
		if (reader->ReadContents())
		{
			if (!reader->ReadUInt64ArrayMap("", shaderCache))
			{
				HL_CORE_ERROR("Could not read ShaderCache!");
				HL_ASSERT(false);
			}
		}
	}
}

