// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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
#include "Engine/Graphics/TextureFormat.h"

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

				case TextureFormat::RED32F:
					return 4;

			}

			HL_ASSERT(false);
			return 0;
		}

		inline uint32 CalculateMipCount(uint32 width, uint32 height)
		{
			return (uint32)(std::floor(std::log2(glm::min(width, height))) + 1);
		}

		inline std::pair<uint32, uint32> GetMipSize(uint32 mip, uint32 width, uint32 height)
		{
			uint32 w = width;
			uint32 h = height;

			while (mip != 0)
			{
				w /= 2;
				h /= 2;
				--mip;
			}

			return { w, h };
		}

		inline uint32 GetImageMemorySize(TextureFormat format, uint32 width, uint32 height)
		{
			return width * height * GetImageFormatBPP(format);
		}

		inline bool IsDepthFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::DEPTH24STENCIL8:
				case TextureFormat::DEPTH32F:
					return true;
			}

			return false;
		}
	}
}
