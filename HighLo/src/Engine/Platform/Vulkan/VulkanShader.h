// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"

#include "Engine/Renderer/Shader.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const ShaderSource &source, const BufferLayout &layout);
		virtual ~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		HLString m_Name;
		HLRendererID m_RendererID = 0;
	};
}

#endif // HIGHLO_API_VULKAN

