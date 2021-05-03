#include "HighLoPch.h"
#include "Framebuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/Rendering/OpenGLFramebuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/Rendering/DX11Framebuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
		{
		#ifdef HIGHLO_API_OPENGL
			return Ref<OpenGLFramebuffer>::Create(spec);
		#elif HIGHLO_API_DX11
			return Ref<DX11Framebuffer>::Create(spec);
		#else
			return nullptr;
		#endif
		}

	FramebufferPool *FramebufferPool::s_Instance = new FramebufferPool;

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