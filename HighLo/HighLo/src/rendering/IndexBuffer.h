#pragma once
#include <core/HLCore.h>
#include <vector>

namespace highlo
{
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual int GetCount() = 0;

		static Ref<IndexBuffer> Create(std::vector<int>& indices);
	};
}
