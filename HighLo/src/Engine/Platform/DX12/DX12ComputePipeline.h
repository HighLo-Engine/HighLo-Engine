// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-03) initial release
//

#pragma once

#include "Engine/Graphics/ComputePipeline.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12ComputePipeline : public ComputePipeline
	{
	public:

		DX12ComputePipeline(const Ref<Shader> &computeShader);
		virtual ~DX12ComputePipeline();

		virtual void Begin(const Ref<CommandBuffer> &renderCommandBuffer = nullptr) override;
		virtual void End() override;

		virtual void Invalidate() override;

		virtual Ref<Shader> GetShader() override { return m_Shader; }

	private:

		Ref<Shader> m_Shader = nullptr;
	};
}

#endif // HIGHLO_API_DX12

