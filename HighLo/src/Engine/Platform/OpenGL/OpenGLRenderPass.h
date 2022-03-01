// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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

		virtual RenderPassSpecification &GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpecification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_OPENGL
