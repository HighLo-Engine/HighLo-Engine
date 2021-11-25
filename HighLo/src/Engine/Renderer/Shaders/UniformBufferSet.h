#pragma once

#include "UniformBuffer.h"

namespace highlo
{
	class UniformBufferSet : public IsSharedReference
	{
	public:

		HLAPI virtual ~UniformBufferSet() = default;

		HLAPI virtual void CreateUniform(uint32 size, uint32 binding) = 0;

		HLAPI virtual Ref<UniformBuffer> GetUniform(uint32 binding, uint32 set = 0, uint32 frame = 0) = 0;
		HLAPI virtual void SetUniform(const Ref<UniformBuffer> &uniform, uint32 set = 0, uint32 frame = 0) = 0;

		HLAPI static Ref<UniformBufferSet> Create(uint32 frames);
	};
}

