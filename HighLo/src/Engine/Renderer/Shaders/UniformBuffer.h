#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/SharedReference.h"

namespace highlo
{
	class UniformBuffer : public IsSharedReference
	{
	public:

		HLAPI virtual ~UniformBuffer() = default;

		HLAPI virtual void SetData(const void *data, uint32 size, uint32 offset = 0) = 0;
		HLAPI virtual uint32 GetBinding() const = 0;

		HLAPI static Ref<UniformBuffer> Create(uint32 size, uint32 binding);
	};
}

