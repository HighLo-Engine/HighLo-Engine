// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DocumentReader.h"

#include "Engine/Platform/RapidJSON/JsonReader.h"
#include "Engine/Platform/RapidXML/XMLReader.h"

namespace highlo
{
	Ref<DocumentReader> DocumentReader::Create(const FileSystemPath &filePath, DocumentType type)
	{
		switch (type)
		{
			case DocumentType::Json:
				return Ref<JsonReader>::Create(filePath);

			case DocumentType::XML:
				return Ref<XMLReader>::Create(filePath);

			case DocumentType::Yaml:
				break;
		}

		// Use default parser
		return Ref<JsonReader>::Create(filePath);
	}
}

