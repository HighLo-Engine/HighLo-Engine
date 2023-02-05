// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Texture2D.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLTexture2D.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Texture2D.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12Texture2D.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanTexture2D.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<Texture2D> Texture2D::LoadFromFile(const FileSystemPath &filePath, bool flipOnLoad)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(filePath, flipOnLoad);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(filePath, flipOnLoad);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(filePath, flipOnLoad);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(filePath, flipOnLoad);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::CreateFromColor(const glm::vec3 &rgb, TextureFormat format)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(rgb, format);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(rgb, format);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(rgb, format);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(rgb, format);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(rgb, width, height, format);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(rgb, width, height, format);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(rgb, width, height, format);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(rgb, width, height, format);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(format, width, height);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(format, width, height);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(format, width, height);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(format, width, height);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(format, width, height, data, props);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(format, width, height, data, props);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(format, width, height, data, props);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(format, width, height, data, props);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::CreateFromSpecification(const TextureSpecification &specification)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(specification);
	#elif HIGHLO_API_DX11
		return Ref<DX11Texture2D>::Create(specification);
	#elif HIGHLO_API_DX12
		return Ref<DX12Texture2D>::Create(specification);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanTexture2D>::Create(specification);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

