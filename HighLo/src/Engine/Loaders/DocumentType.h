// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-01-10) initial release
//

#pragma once

namespace highlo
{
	enum class DocumentType
	{
		None = 0,
		Json,
		XML,
		Yaml
	};

	enum class DocumentDataType
	{
		None = 0,
		Int32, UInt32,
		Int64, UInt64,
		Float, Double,
		Vec2, Vec3, Vec4,
		Mat2, Mat3, Mat4,
		Bool,
		Quat,
		String
	};
}

