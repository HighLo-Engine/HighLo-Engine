// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DocumentWriter.h"

#include "Engine/Platform/JSON/JsonWriter.h"

namespace highlo
{
	Ref<DocumentWriter> DocumentWriter::Create(const FileSystemPath &filePath, DocumentType type)
	{
		if (type != DocumentType::None)
		{
			switch (type)
			{
				case DocumentType::Json:
					return Ref<JSONWriter>::Create(filePath);

				case DocumentType::XML:
					break;

				case DocumentType::Yaml:
					break;
			}
		}

		// Use default parser
		return Ref<JSONWriter>::Create(filePath);
	}
}

