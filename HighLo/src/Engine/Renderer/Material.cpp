// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Material.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLMaterial.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanMaterial.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Material.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	Ref<Material> Material::Create(const Ref<Shader> &shader, const HLString &name)
	{
#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLMaterial>::Create(shader, name);
#elif HIGHLO_API_VULKAN
		return Ref<VulkanMaterial>::Create(shader, name);
#elif HIGHLO_API_DX11
		return Ref<DX11Material>::Create(shader, name);
#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
#else
		HL_ASSERT(false);
		return nullptr;
#endif // HIGHLO_API_OPENGL
	}

	Ref<Material> Material::Copy(const Ref<Material> &other, const HLString &name)
	{
#ifdef HIGHLO_API_OPENGL
		return Ref<OpenGLMaterial>::Create(other, name);
#elif HIGHLO_API_VULKAN
		return Ref<VulkanMaterial>::Create(other, name);
#elif HIGHLO_API_DX11
		return Ref<DX11Material>::Create(other, name);
#elif HIGHLO_API_DX12
		HL_ASSERT(false);
		return nullptr;
#else
		HL_ASSERT(false);
		return nullptr;
#endif // HIGHLO_API_OPENGL
	}
}

