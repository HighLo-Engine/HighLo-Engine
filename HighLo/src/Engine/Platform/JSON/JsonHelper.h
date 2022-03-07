// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Loaders/DocumentType.h"

#include <rapidjson/document.h>

namespace highlo::utils
{
	static std::pair<rapidjson::Value, rapidjson::Value> ConvertDocumentTypeToRenderableFormat(rapidjson::Document &document, DocumentDataType type)
	{
		std::pair<rapidjson::Value, rapidjson::Value> result;
		rapidjson::Value keyType(rapidjson::kStringType);
		keyType.SetString("type", document.GetAllocator());

		rapidjson::Value valType(rapidjson::kStringType);

		HLString typeStr = utils::DocumentDataTypeToString(type);
		valType.SetString(*typeStr, typeStr.Length(), document.GetAllocator());

		result.first = keyType;
		result.second = valType;
		return result;
	}
}

