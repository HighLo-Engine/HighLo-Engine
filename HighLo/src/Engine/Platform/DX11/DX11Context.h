// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/RenderingContext.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"
#include "Engine/Window/Window.h"

namespace highlo
{
	class DX11Context : public RenderingContext
	{
	public:
		DX11Context(WindowData& window_properties, HWND hwnd);
		virtual ~DX11Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

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

		bool							m_VSyncEnabled = false;

	private:
		std::string m_InitSuccessString = "";
	};
}
#endif // HIGHLO_API_DX11
