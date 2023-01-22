// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Framebuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLFramebuffer.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Framebuffer.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12Framebuffer.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanFramebuffer.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
	FramebufferPool *FramebufferPool::s_Instance = new FramebufferPool();

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
	{
	#ifdef HIGHLO_API_OPENGL
			return Ref<OpenGLFramebuffer>::Create(spec);
	#elif HIGHLO_API_DX11
		return Ref<DX11Framebuffer>::Create(spec);
	#elif HIGHLO_API_DX12
		return Ref<DX12Framebuffer>::Create(spec);
	#elif HIGHLO_API_VULKAN
		return Ref<VulkanFramebuffer>::Create(spec);
	#else
		HL_ASSERT(false);
		return nullptr;
	#endif
	}

	FramebufferPool::FramebufferPool(uint32 maxFbs)
		: m_MaxFramebufferCount(maxFbs) {}

	FramebufferPool::~FramebufferPool()
	{
		m_Pool.clear();
		m_Pool.shrink_to_fit();
	}

	WeakRef<Framebuffer> FramebufferPool::AllocateBuffer()
	{
		return WeakRef<Framebuffer>();
	}

	void FramebufferPool::Add(const Ref<Framebuffer> &framebuffer)
	{
		if (m_MaxFramebufferCount > m_Pool.size())
			m_Pool.push_back(framebuffer);
	}
}

