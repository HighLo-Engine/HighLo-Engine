// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <math.h>
#include <cmath>

#include "Engine/Core/Core.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"
#include "Engine/Renderer/TextureFormat.h"

namespace highlo
{
	namespace utils
	{
		inline uint32 GetImageFormatBPP(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:
				case TextureFormat::SRGB:
					return 3;

				case TextureFormat::RGBA:
					return 4;

				case TextureFormat::RGBA16F:
					return 2 * 4;

				case TextureFormat::RGBA32F:
					return 4 * 4;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline uint32 CalculateMipCount(uint32 width, uint32 height)
		{
			return (int32)(std::floor(std::log2(glm::min(width, height))) + 1);
		}

		inline uint32 GetImageMemorySize(TextureFormat format, uint32 width, uint32 height)
		{
			return width * height * GetImageFormatBPP(format);
		}
	}
}