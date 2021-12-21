#pragma once

#include "Engine/Renderer/RenderPass.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11RenderPass : public RenderPass
	{
	public:

		DX11RenderPass(const RenderPassSpecification &spec);
		virtual ~DX11RenderPass();

		virtual RenderPassSpecification &GetSpecification() override { return m_Specification; }
		virtual const RenderPassSpecification &GetSpecification() const override { return m_Specification; }

	private:

		RenderPassSpecification m_Specification;
	};
}

#endif // HIGHLO_API_DX11

