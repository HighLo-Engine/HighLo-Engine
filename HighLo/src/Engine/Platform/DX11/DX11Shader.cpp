#include "HighLoPch.h"
#include "DX11Shader.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Core/HLLog.h"

namespace highlo
{
	static DXGI_FORMAT ShaderDataTypeToDxgiFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Mat3:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Mat4:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:      return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
		case ShaderDataType::Bool:     return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}

		return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	static void GetDX11VertexShaderLayout(const BufferLayout& layout, std::vector<D3D11_INPUT_ELEMENT_DESC>& _Out_ desc, UINT& _Out_ elem_count)
	{
		for (auto& element : layout)
		{
			D3D11_INPUT_CLASSIFICATION Classification = D3D11_INPUT_PER_VERTEX_DATA;
			UINT InstanceDataStepRate = 0;

			if (element.ElementInputClass == BufferElementInputClass::INPUT_PER_INSTANCE_DATA)
			{
				Classification = D3D11_INPUT_PER_INSTANCE_DATA;
				InstanceDataStepRate = 1;
			}

			D3D11_INPUT_ELEMENT_DESC InputElemDesc;
			ZeroMemory(&InputElemDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

			InputElemDesc.SemanticName = element.Name.C_Str();
			InputElemDesc.SemanticIndex = 0;
			InputElemDesc.Format = ShaderDataTypeToDxgiFormat(element.Type);
			InputElemDesc.InputSlot = element.InputSlot;
			InputElemDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			InputElemDesc.InputSlotClass = Classification;
			InputElemDesc.InstanceDataStepRate = InstanceDataStepRate;

			desc.push_back(InputElemDesc);
		}

		elem_count = (UINT)layout.GetElements().size();
	}
	
	DX11VertexShader::DX11VertexShader(const HLString& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.C_Str(),
			src.Length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			HL_CORE_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DX11Resources::s_Device->CreateVertexShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile vertex shader");
			HL_CORE_ERROR(error_msg);
			return;
		}
	}

	void DX11VertexShader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count)
	{
		HRESULT result = DX11Resources::s_Device->CreateInputLayout(layout_description, element_count, m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), m_InputLayout.GetAddressOf());
		if (FAILED(result))
		{
			HL_CORE_ERROR("Failed to create ID3D11InputLayout");
			return;
		}
	}
	
	DX11HullShader::DX11HullShader(const HLString& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.C_Str(),
			src.Length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "hs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			HL_CORE_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DX11Resources::s_Device->CreateHullShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile hull shader");
			HL_CORE_ERROR(error_msg);
			return;
		}
	}
	
	DX11DomainShader::DX11DomainShader(const HLString& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.C_Str(),
			src.Length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "ds_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			HL_CORE_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DX11Resources::s_Device->CreateDomainShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile domain shader");
			HL_CORE_ERROR(error_msg);
			return;
		}
	}
	
	DX11PixelShader::DX11PixelShader(const HLString& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.C_Str(),
			src.Length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			HL_CORE_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DX11Resources::s_Device->CreatePixelShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile pixel shader");
			HL_CORE_ERROR(error_msg);
			return;
		}
	}

	DX11Shader::DX11Shader(const ShaderSource& source, const BufferLayout& layout)
	{
		m_VertexShader = std::make_unique<DX11VertexShader>(source.m_VertexShaderSrc);
		m_PixelShader = std::make_unique<DX11PixelShader>(source.m_PixelShaderSrc);

		if (!source.m_TessellationControlShaderSrc.IsEmpty())
			m_HullShader = std::make_unique<DX11HullShader>(source.m_TessellationControlShaderSrc);

		if (!source.m_TessellationEvaluationShaderSrc.IsEmpty())
			m_DomainShader = std::make_unique<DX11DomainShader>(source.m_TessellationEvaluationShaderSrc);

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3d11layout;
		UINT element_count = 0;
		GetDX11VertexShaderLayout(layout, d3d11layout, element_count);

		CreateInputLayout(&d3d11layout[0], element_count);
	}

	void DX11Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count)
	{
		m_VertexShader->CreateInputLayout(layout_description, element_count);
	}

	void DX11Shader::Bind() const
	{
		DX11Resources::s_DeviceContext->IASetInputLayout(m_VertexShader->GetInputLayout());

		DX11Resources::s_DeviceContext->VSSetShader(m_VertexShader->GetShader(), NULL, 0);
		DX11Resources::s_DeviceContext->PSSetShader(m_PixelShader->GetShader(), NULL, 0);

		if (m_HullShader)
			DX11Resources::s_DeviceContext->HSSetShader(m_HullShader->GetShader(), NULL, 0);
		else
			DX11Resources::s_DeviceContext->HSSetShader(nullptr, NULL, 0);

		if (m_DomainShader)
			DX11Resources::s_DeviceContext->DSSetShader(m_DomainShader->GetShader(), NULL, 0);
		else
			DX11Resources::s_DeviceContext->DSSetShader(nullptr, NULL, 0);

		DX11Resources::s_DeviceContext->PSSetSamplers(0, 1, DX11Resources::s_SamplerState.GetAddressOf());
		DX11Resources::s_DeviceContext->PSSetSamplers(1, 1, DX11Resources::s_SamplerState.GetAddressOf());
		DX11Resources::s_DeviceContext->PSSetSamplers(2, 1, DX11Resources::s_SamplerState.GetAddressOf());
		DX11Resources::s_DeviceContext->PSSetSamplers(3, 1, DX11Resources::s_SamplerState.GetAddressOf());
		DX11Resources::s_DeviceContext->PSSetSamplers(4, 1, DX11Resources::s_SamplerState.GetAddressOf());

		if (m_HullShader)
		{
			DX11Resources::s_DeviceContext->HSSetSamplers(0, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->HSSetSamplers(1, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->HSSetSamplers(2, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->HSSetSamplers(3, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->HSSetSamplers(4, 1, DX11Resources::s_SamplerState.GetAddressOf());
		}

		if (m_DomainShader)
		{
			DX11Resources::s_DeviceContext->DSSetSamplers(0, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->DSSetSamplers(1, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->DSSetSamplers(2, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->DSSetSamplers(3, 1, DX11Resources::s_SamplerState.GetAddressOf());
			DX11Resources::s_DeviceContext->DSSetSamplers(4, 1, DX11Resources::s_SamplerState.GetAddressOf());
		}
	}
}

#endif
