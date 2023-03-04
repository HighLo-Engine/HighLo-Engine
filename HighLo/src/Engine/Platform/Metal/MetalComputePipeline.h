// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/ComputePipeline.h"

namespace highlo
{
	class MetalComputePipeline : public ComputePipeline
	{
	public:

		MetalComputePipeline(const Ref<Shader> &computeShader);
		virtual ~MetalComputePipeline();

		virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

	private:

		Ref<Shader> m_Shader = nullptr;
	};
}

#endif // HIGHLO_API_METAL

