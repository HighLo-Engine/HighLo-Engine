// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DocumentReader.h"

#include "Engine/Platform/JSON/JsonReader.h"

namespace highlo
{
	Ref<DocumentReader> DocumentReader::Create(const FileSystemPath &filePath, DocumentType type)
	{
		switch (type)
		{
			case DocumentType::Json:
				return Ref<JsonReader>::Create(filePath);

			case DocumentType::XML:
				break;

			case DocumentType::Yaml:
				break;
		}

		// Use default parser
		return Ref<JsonReader>::Create(filePath);
	}
}

