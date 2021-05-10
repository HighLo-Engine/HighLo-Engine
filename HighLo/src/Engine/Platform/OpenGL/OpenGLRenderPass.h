#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Renderer/RenderPass.h"

namespace highlo
{
	class OpenGLRenderPass : public RenderPass
	{
	public:

		OpenGLRenderPass(const RenderPassSpecification &spec);
		virtual ~OpenGLRenderPass();

		virtual RenderPassSpecification &GetSpcification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpcification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_OPENGL