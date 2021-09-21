// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Texture.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLTexture.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Texture.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	Ref<Texture> Texture2D::LoadFromFile(const HLString &filepath, TextureFormat format, bool flip_on_load)
	{
		return Ref<Texture>(OpenGLTexture2D::LoadFromFile(filepath, format, flip_on_load));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3 &rgb, TextureFormat format)
	{
		return Ref<Texture>(OpenGLTexture2D::CreateFromColor(rgb, format));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
		return Ref<Texture>(OpenGLTexture2D::CreateFromColor(rgb, width, height, format));
	}

	Ref<Texture> Texture2D::Create(TextureFormat format, uint32 width, uint32 height)
	{
		return Ref<OpenGLTexture2D>::Create(format, width, height);
	}

	Ref<Texture> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
		return Ref<OpenGLTexture2D>::Create(format, width, height, data, props);
	}

	Ref<Texture> Texture3D::LoadFromFiles(const std::vector<HLString> &filepaths)
	{
		return Ref<OpenGLTexture3D>::Create(filepaths);
	}
	
	Ref<Texture> Texture3D::Create(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
		return Ref<OpenGLTexture3D>::Create(format, width, height, data);
	}

#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	Ref<Texture> Texture2D::LoadFromFile(const HLString &filepath, TextureFormat format, bool flip_on_load)
	{
		return Ref<Texture>(DX11Texture2D::LoadFromFile(filepath, format, flip_on_load));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3 &rgb, TextureFormat format)
	{
		return Ref<Texture>(DX11Texture2D::CreateFromColor(rgb, format));
	}

	Ref<Texture> Texture2D::CreateFromColor(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
		return Ref<Texture>(DX11Texture2D::CreateFromColor(rgb, width, height, format));
	}

	Ref<Texture> Texture2D::Create(TextureFormat format, uint32 width, uint32 height)
	{
		return Ref<DX11Texture2D>::Create(format, width, height);
	}

	Ref<Texture> Texture2D::Create(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
		return Ref<DX11Texture2D>::Create(format, width, height, data, props);
	}

	Ref<Texture> Texture3D::LoadFromFiles(const std::vector<HLString> &filepaths)
	{
		// TODO
		return nullptr;
	}

	Ref<Texture> Texture3D::Create(TextureFormat format, uint32 width, uint32 height, const void *data)
	{
		// TODO
		return nullptr
	}
#endif // HIGHLO_API_DX11

}
