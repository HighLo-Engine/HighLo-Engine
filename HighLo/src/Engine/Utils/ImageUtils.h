#pragma once

#include <glm/glm.hpp>
#include <math.h>
#include <cmath>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Core/HLAssert.h"
#include "Engine/Renderer/ImageFormat.h"

namespace highlo
{
	namespace utils
	{
		inline uint32 GetImageFormatBPP(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB:
				case ImageFormat::SRGB:
					return 3;

				case ImageFormat::RGBA:
					return 4;

				case ImageFormat::RGBA16F:
					return 2 * 4;

				case ImageFormat::RGBA32F:
					return 4 * 4;
			}

			HL_ASSERT(false);
			return 0;
		}

		inline uint32 CalculateMipCount(uint32 width, uint32 height)
		{
			return (int32)(std::floor(std::log2(glm::min(width, height))) + 1);
		}

		inline uint32 GetImageMemorySize(ImageFormat format, uint32 width, uint32 height)
		{
			return width * height * GetImageFormatBPP(format);
		}
	}
}