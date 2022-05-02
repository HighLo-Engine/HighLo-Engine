// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-08) initial release
//

#pragma once

#include "Engine/Graphics/ComputePipeline.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLComputePipeline : public ComputePipeline
	{
	public:

		OpenGLComputePipeline(const Ref<Shader> &computeShader);
		virtual ~OpenGLComputePipeline();

		virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

	private:

		Ref<Shader> m_Shader = nullptr;
	};
}

#endif // HIGHLO_API_OPENGL

