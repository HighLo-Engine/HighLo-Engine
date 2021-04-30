#pragma once
#include <rendering/RenderingContext.h>

#ifdef HIGHLO_API_DX11
#include <api/dx11/DX11Resources.h>
#include <api/dx11/window/DX11Window.h>

namespace highlo
{
	class DX11Context : public RenderingContext
	{
	public:
		DX11Context(WindowData& window_properties, HWND hwnd);
		~DX11Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		void InitializeSwapChain();
		void InitializeRenderTargetView();
		void InitializeDepthStencilViewAndBuffer();
		void InitializeDepthStencilState();
		void InitializeViewPort();
		void InitializeRasterizerState();
		void InitializeBlendState();
		void InitializeSamplerState();
		void InitializeAudioEngine();
		void ReleaseAudioEngine();

	private:
		WindowData m_WindowProperties;
		HWND m_hwnd;

		ComPtr<IDXGISwapChain>			m_SwapChain;
		ComPtr<ID3D11Texture2D>			m_DepthStencilBuffer;
		ComPtr<ID3D11BlendState>		m_BlendState;

	private:
		std::string m_InitSuccessString = "";
	};
}
#endif // HIGHLO_API_DX11
