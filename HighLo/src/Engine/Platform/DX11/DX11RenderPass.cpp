#include "HighLoPch.h"
#include "DX11RenderPass.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11RenderPass::DX11RenderPass(const RenderPassSpecification &spec)
		: m_Specification(spec)
	{
	}

	DX11RenderPass::~DX11RenderPass()
	{
	}
}

#endif // HIGHLO_API_DX11

