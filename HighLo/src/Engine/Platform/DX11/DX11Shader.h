// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Renderer/Shaders/Shader.h"

#ifdef HIGHLO_API_DX11

#include "DX11Resources.h"

namespace highlo
{
	class DX11VertexShader
	{
	public:
		DX11VertexShader(const HLString& src);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count);

		inline ID3D11VertexShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }
		inline ID3D11InputLayout* GetInputLayout() { return m_InputLayout.Get(); }

	private:
		ComPtr<ID3D11VertexShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
		ComPtr<ID3D11InputLayout>	m_InputLayout = nullptr;
	};

	class DX11HullShader
	{
	public:
		DX11HullShader(const HLString& src);

		inline ID3D11HullShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11HullShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DX11DomainShader
	{
	public:
		DX11DomainShader(const HLString& src);

		inline ID3D11DomainShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11DomainShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DX11PixelShader
	{
	public:
		DX11PixelShader(const HLString& src);

		inline ID3D11PixelShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11PixelShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const ShaderSource& source, const BufferLayout& layout);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count);

		inline DX11VertexShader* GetVertexShader() { return m_VertexShader.get(); }
		inline DX11PixelShader* GetPixelShader() { return m_PixelShader.get(); }

		virtual void Bind() const override;
		virtual HLRendererID GetRendererID() override;

	protected:
		std::unique_ptr<DX11VertexShader>	m_VertexShader;
		std::unique_ptr<DX11HullShader>		m_HullShader = nullptr;		// optional
		std::unique_ptr<DX11DomainShader>	m_DomainShader = nullptr;	// optional
		std::unique_ptr<DX11PixelShader>	m_PixelShader;
	};
}

#endif
