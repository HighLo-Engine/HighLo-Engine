#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <xaudio2.h>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace highlo
{
	class DX11Resources
	{
	public:
		static ComPtr<ID3D11Device>				s_Device;
		static ComPtr<ID3D11DeviceContext>		s_DeviceContext;
		static ComPtr<ID3D11RenderTargetView>	s_RenderTargetView;
		static ComPtr<ID3D11RasterizerState>	s_RasterizerState;
		static ComPtr<ID3D11DepthStencilState>	s_DepthStencilState;
		static ComPtr<ID3D11DepthStencilView>	s_DepthStencilView;
		static ComPtr<ID3D11SamplerState>		s_SamplerState;
		static ComPtr<IXAudio2>					s_XAudio2;
		static IXAudio2MasteringVoice* s_XAudio2MasteringVoice;
	};
}
#endif // HIGHLO_API_DX11
