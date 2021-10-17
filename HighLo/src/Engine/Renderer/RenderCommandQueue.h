// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-17) initial release
//

#pragma once

namespace highlo
{
	class RenderCommandQueue
	{
	public:

		typedef void(*RenderCommandFn)(void*);

		HLAPI RenderCommandQueue();
		HLAPI ~RenderCommandQueue();

		HLAPI void *Allocate(RenderCommandFn func, uint32 size);
		HLAPI void Execute();

	private:

		uint8 *m_CommandBuffer;
		uint8 *m_CommandBufferPtr;
		uint32 m_CommandCount = 0;
	};
}

