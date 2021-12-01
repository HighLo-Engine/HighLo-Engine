// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Texture.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLTexture.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Texture.h"
#elif HIGHLO_API_VULKAN

#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<Texture2D> Texture2D::LoadFromFile(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(filePath, format, flipOnLoad);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::CreateFromColor(const glm::vec3 &rgb, TextureFormat format)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(rgb, format);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(rgb, width, height, format);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, bool waitUntilRenderQueueSubmission)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(format, width, height, waitUntilRenderQueueSubmission);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture2D>::Create(format, width, height, data, props);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}



	Ref<Texture3D> Texture3D::LoadFromFiles(const std::vector<HLString> &filepaths, bool flipOnLoad)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture3D>::Create(filepaths, flipOnLoad);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Texture3D> Texture3D::Create(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
	#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLTexture3D>::Create(format, width, height, data);
	#elif HIGHLO_API_DX11
		return nullptr;
	#elif HIGHLO_API_DX12
		return nullptr;
	#elif HIGHLO_API_VULKAN
		return nullptr;
	#else
		return nullptr;
	#endif // HIGHLO_API_OPENGL
	}
}

