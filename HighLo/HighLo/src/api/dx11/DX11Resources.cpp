#include "DX11Resources.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	ComPtr<ID3D11Device>			DX11Resources::s_Device = nullptr;
	ComPtr<ID3D11DeviceContext>		DX11Resources::s_DeviceContext = nullptr;
	ComPtr<ID3D11RenderTargetView>	DX11Resources::s_RenderTargetView = nullptr;
	ComPtr<ID3D11RasterizerState>	DX11Resources::s_RasterizerState = nullptr;
	ComPtr<ID3D11DepthStencilState> DX11Resources::s_DepthStencilState = nullptr;
	ComPtr<ID3D11DepthStencilView>	DX11Resources::s_DepthStencilView = nullptr;
	ComPtr<ID3D11SamplerState>		DX11Resources::s_SamplerState = nullptr;
	ComPtr<IXAudio2>				DX11Resources::s_XAudio2 = nullptr;
	IXAudio2MasteringVoice*			DX11Resources::s_XAudio2MasteringVoice = nullptr;
}
#endif // HIGHLO_API_DX11
