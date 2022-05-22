// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DocumentWriter.h"

#include "Engine/Platform/RapidJSON/JsonWriter.h"
#include "Engine/Platform/RapidXML/XMLWriter.h"

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
			}
		}

		// Use default parser
		return Ref<JSONWriter>::Create(filePath);
	}
}

