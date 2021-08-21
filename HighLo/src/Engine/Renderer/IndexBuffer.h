#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	class IndexBuffer : public IsSharedReference
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() = 0;
		virtual void UpdateContents(void *data, uint32 size) = 0;
		virtual void UpdateContents(std::vector<int32> &indices) = 0;

		virtual HLRendererID GetRendererID() = 0;

		static Ref<IndexBuffer> Create(std::vector<int32>& indices);
		static Ref<IndexBuffer> Create(void *data, uint32 size);
		static Ref<IndexBuffer> Create(uint32 size);
	};
}
