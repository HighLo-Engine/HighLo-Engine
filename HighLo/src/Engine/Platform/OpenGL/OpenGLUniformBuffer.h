// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Renderer/UniformBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const HLString& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32 slot = 0);
		~OpenGLUniformBuffer();

		virtual void UploadToShader() override;

	private:
		uint32 m_ID = 0;
	};
}

#endif
