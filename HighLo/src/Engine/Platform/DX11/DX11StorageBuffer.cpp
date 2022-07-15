#include "HighLoPch.h"
#include "DX11StorageBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11StorageBuffer::DX11StorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
	}

	DX11StorageBuffer::~DX11StorageBuffer()
	{
	}

	void DX11StorageBuffer::Bind() const
	{
	}

	void DX11StorageBuffer::Unbind() const
	{
	}

	void DX11StorageBuffer::UploadToShader()
	{
	}

	void DX11StorageBuffer::Resize(uint32 size)
	{
	}
}

#endif // HIGHLO_API_DX11

