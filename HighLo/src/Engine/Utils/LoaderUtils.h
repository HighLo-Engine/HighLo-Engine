// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-11) initial release
//

#pragma once

#include "Engine/Loaders/DocumentType.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo::utils
{
	static HLString DocumentDataTypeToString(DocumentDataType type)
	{
		HLString typeStr = "unknown";
		switch (type)
		{
			case DocumentDataType::None:
				typeStr = "unknown";
				break;

			case DocumentDataType::Int32:
				typeStr = "int32";
				break;

			case DocumentDataType::UInt32:
				typeStr = "uint32";
				break;

			case DocumentDataType::Int64:
				typeStr = "int64";
				break;

			case DocumentDataType::UInt64:
				typeStr = "uint64";
				break;

			case DocumentDataType::Float:
				typeStr = "float";
				break;

			case DocumentDataType::Double:
				typeStr = "double";
				break;

			case DocumentDataType::Vec2:
				typeStr = "vec2";
				break;

			case DocumentDataType::Vec3:
				typeStr = "vec3";
				break;

			case DocumentDataType::Vec4:
				typeStr = "vec4";
				break;

			case DocumentDataType::Mat2:
				typeStr = "mat2";
				break;

			case DocumentDataType::Mat3:
				typeStr = "mat3";
				break;

			case DocumentDataType::Mat4:
				typeStr = "mat4";
				break;

			case DocumentDataType::Bool:
				typeStr = "bool";
				break;

			case DocumentDataType::Quat:
				typeStr = "quat";
				break;

			case DocumentDataType::String:
				typeStr = "string";
				break;

			default:
				typeStr = "unknown";
				break;
		}

		return typeStr;
	}


}

