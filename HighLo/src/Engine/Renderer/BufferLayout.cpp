#include "HighLoPch.h"
#include "BufferLayout.h"

namespace highlo
{
	BufferLayout BufferLayout::Empty = std::initializer_list<BufferElement>();

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32 offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}
