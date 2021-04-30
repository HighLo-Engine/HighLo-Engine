#include "HighLoPch.h"
#include "Framebuffer.h"

#ifdef HIGHLO_API_OPENGL
#include "engine/api/opengl/rendering/OpenGLFramebuffer.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "engine/api/dx11/rendering/DX11Framebuffer.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
		{
		#ifdef HIGHLO_API_OPENGL
			return MakeRef<OpenGLFramebuffer>(spec);
		#elif HIGHLO_API_DX11
			return MakeRef<DX11Framebuffer>(spec);
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