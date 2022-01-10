// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DocumentWriter.h"

#include "Engine/Platform/JSON/JsonWriter.h"
#include "Engine/Platform/XML/XMLWriter.h"
#include "Engine/Platform/YAML/YAMLWriter.h"

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
					return Ref<XMLWriter>::Create(filePath);

				case DocumentType::Yaml:
					return Ref<YAMLWriter>::Create(filePath);
			}
		}

		// Use default parser
		return Ref<JSONWriter>::Create(filePath);
	}
}

