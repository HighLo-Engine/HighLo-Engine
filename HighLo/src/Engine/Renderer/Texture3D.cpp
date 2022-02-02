// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Texture3D.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLTexture3D.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Texture3D.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12Texture3D.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanTexture3D.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<Texture3D> Texture3D::LoadFromFiles(const FileSystemPath &filepath, bool flipOnLoad)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture3D>::Create(filepath, flipOnLoad);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture3D>::Create(filepath, flipOnLoad);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture3D>::Create(filepath, flipOnLoad);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture3D>::Create(filepath, flipOnLoad);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture3D> Texture3D::Create(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture3D>::Create(format, width, height, data);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture3D>::Create(format, width, height, data);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture3D>::Create(format, width, height, data);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture3D>::Create(format, width, height, data);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

