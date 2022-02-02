// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11UniformBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{	
	DX11UniformBuffer::DX11UniformBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(size + (16 - (size % 16)));
		desc.StructureByteStride = 0;

		DX11Resources::s_Device->CreateBuffer(&desc, 0, m_Buffer.GetAddressOf());
	}
	
	DX11UniformBuffer::~DX11UniformBuffer()
	{
	}

	void DX11UniformBuffer::Bind() const
	{
	}

	void DX11UniformBuffer::Unbind() const
	{
	}
	
	void DX11UniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		// TODO
	}
}

#endif // HIGHLO_API_DX11
