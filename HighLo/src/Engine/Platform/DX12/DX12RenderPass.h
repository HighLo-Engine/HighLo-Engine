// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Graphics/RenderPass.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12RenderPass : public RenderPass
	{
	public:

		DX12RenderPass(const RenderPassSpecification &specification);
		virtual ~DX12RenderPass();

		virtual RenderPassSpecification &GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpecification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_DX12

