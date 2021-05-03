#pragma once

#include "Engine/Core/HLCore.h"

namespace highlo
{
	class IndexBuffer : public IsSharedReference
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual int32 GetCount() = 0;

		static Ref<IndexBuffer> Create(std::vector<int32>& indices);
	};
}
