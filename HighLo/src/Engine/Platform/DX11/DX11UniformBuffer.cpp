// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11UniformBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11UniformBuffer::DX11UniformBuffer(const HLString& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32 slot)
		: UniformBuffer(name, layout, parent, slot)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(m_DataSize + (16 - (m_DataSize % 16)));
		desc.StructureByteStride = 0;

		DX11Resources::s_Device->CreateBuffer(&desc, 0, m_Buffer.GetAddressOf());
	}

	void DX11UniformBuffer::UploadToShader()
	{
		D3D11_MAPPED_SUBRESOURCE mr;
		HRESULT result = DX11Resources::s_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
		if (FAILED(result))
		{
			HL_CORE_ERROR("Failed to map constant buffer");
			return;
		}

		CopyMemory(mr.pData, m_Data, m_DataSize);
		DX11Resources::s_DeviceContext->Unmap(m_Buffer.Get(), 0);

		switch (m_ParentShader)
		{
		case UniformBufferParentShader::VERTEX_SHADER: { DX11Resources::s_DeviceContext->VSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::HULL_SHADER:   { DX11Resources::s_DeviceContext->HSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::DOMAIN_SHADER: { DX11Resources::s_DeviceContext->DSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		case UniformBufferParentShader::PIXEL_SHADER:  { DX11Resources::s_DeviceContext->PSSetConstantBuffers(BindingSlot, 1, m_Buffer.GetAddressOf()); break; }
		}
	}
}

#endif // HIGHLO_API_DX11
