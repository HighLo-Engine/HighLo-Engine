// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Math/Math.h"
#include "Engine/Core/Allocator.h"
#include "Engine/Core/FileSystemPath.h"
#include "TextureFormat.h"
#include "Engine/Assets/Asset.h"

namespace highlo
{
	class Texture : public Asset
	{
	public:

		HLString Name = "";

		HLAPI virtual ~Texture() {}
		HLAPI virtual uint32 GetDimensions() const = 0;
		HLAPI virtual TextureType GetType() const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;
		HLAPI virtual Allocator GetData() = 0;

		HLAPI virtual void Bind(uint32 slot) const = 0;
		HLAPI virtual void Unbind(uint32 slot) const = 0;

		HLAPI virtual void Release() = 0;
		HLAPI virtual void Invalidate() = 0;
		HLAPI virtual bool IsLoaded() const = 0;

		HLAPI virtual void Resize(const glm::uvec2 &size) = 0;
		HLAPI virtual void Resize(const uint32 width, const uint32 height) = 0;

		HLAPI virtual void Lock() = 0;
		HLAPI virtual void Unlock() = 0;

		HLAPI virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) = 0;
		HLAPI virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) = 0;
		HLAPI virtual void UpdateResourceData(void *data) = 0;
		HLAPI virtual void UpdateResourceData() = 0;
		HLAPI virtual uint32 GetMipLevelCount() = 0;
		HLAPI virtual std::pair<uint32, uint32> GetMipSize(uint32 mip) = 0;
		HLAPI virtual void GenerateMips(bool readonly = false) = 0;
		HLAPI virtual float GetAspectRatio() const = 0;

		HLAPI virtual TextureFormat GetFormat() = 0;
		HLAPI inline HLRendererID GetRendererID() const { return RendererID; }
		HLAPI inline uint64 GetHash() const { return (uint64)RendererID; }

		HLAPI virtual TextureSpecification &GetSpecification() = 0;
		HLAPI virtual const TextureSpecification &GetSpecification() const = 0;

		HLAPI inline bool operator==(const Texture &other) const { return RendererID == other.RendererID; }
		HLAPI inline bool operator!=(const Texture &other) const { return !(*this == other); }

	protected:

		HLRendererID RendererID = 0;
	};
}

