#pragma once

#include "Engine/Window/Window.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

#include <Windows.h>
#include "Engine/Renderer/RenderingContext.h"

namespace highlo
{
	class DX11Window;

	struct WindowCallbackData
	{
		DX11Window* p_EngineWindow = nullptr;
		EventCallbackFn EventCallback;
	};

	class DX11Window : public Window
	{
	public:
		DX11Window(const WindowData& properties);
		~DX11Window();

		virtual void SetEventCallback(const EventCallbackFn& callback) override;

		virtual void Update() override;

		virtual unsigned int GetWidth() override;
		virtual unsigned int GetHeight() override;
		virtual void* GetNativeHandle() override { return (void*)m_NativeHandle; }

		virtual void SetCursorLocked(bool bLocked) override;
		bool IsCursorLocked() override;

		bool IsFocused() override;

		// Specific to DX11Window
		void OnResize(unsigned int width, unsigned int height);

	private:
		WindowData m_Properties;
		void Init();

		std::unique_ptr<RenderingContext> m_Context;

	private:
		HWND m_NativeHandle = nullptr;
		bool m_CursorLocked = false;

	private:
		WindowCallbackData m_CallbackData;
	};
}
#endif // HIGHLO_API_DX11
