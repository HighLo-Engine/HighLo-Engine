#pragma once

namespace highlo
{
	class SwapChain : public IsSharedReference
	{
	public:

		HLAPI virtual ~SwapChain() = default;



		HLAPI static Ref<SwapChain> Create();
	};
}

