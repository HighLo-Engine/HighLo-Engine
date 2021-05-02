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

		virtual uint32 GetWidth() override { return m_Properties.m_Width; }
		virtual uint32 GetHeight() override { return m_Properties.m_Height; }
		virtual void* GetNativeHandle() override { return (void*)m_NativeHandle; }

		virtual void SetWindowIcon(const HLString &path) override;
		virtual std::pair<int32, int32> GetWindowDimensions() override;
		virtual std::pair<int32, int32> GetWindowPosition() override;
		virtual void CloseWindow() override;

		virtual int32 ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType = WindowMessageButtonType::None, WindowMessageIcon icon = WindowMessageIcon::None) override;
		virtual void SetMenuBar(const Ref<MenuBar> &bar) override;

		virtual void SetVSync(bool bEnabled) override;
		virtual void SetVisible(bool bVisible) override;
		virtual void SetFocus(bool bEnabled) override;
		virtual void SetFullscreen(bool bEnabled) override;
		virtual void ShowCursor() override;
		virtual void HideCursor() override;
		virtual void Maximize() override;
		virtual void CenterWindow() override;
		virtual void SetTitle(const HLString &title) override;

		virtual bool IsVisible() override { return m_Properties.m_Visible; }
		virtual bool IsCursorHidden() override { return !m_Properties.m_CursorVisible; }
		virtual bool IsMaximized() override { return m_Properties.m_Maximized; }
		virtual bool IsFullscreen() override { return m_Properties.m_Fullscreen; }
		virtual bool IsCentered() override { return m_Properties.m_Centered; }
		virtual const HLString &GetTitle() override { return m_Properties.m_Title; }
		bool IsFocused() override { return m_Properties.m_Focused; }

		// Specific to DX11Window
		void OnResize(uint32 width, uint32 height);

	private:

		struct WNDPlacement
		{
			int32 WindowWidth = 0, WindowHeight = 0;
			WINDOWPLACEMENT Placement;
		};

	private:
		WindowData m_Properties;
		static WNDPlacement m_Placement;

		void Init();

		std::unique_ptr<RenderingContext> m_Context;

	private:
		HWND m_NativeHandle = nullptr;
		HICON m_WindowIcon = NULL;
		HCURSOR m_Cursor = NULL;
		Ref<MenuBar> m_MenuBar;
		bool m_CursorLocked = false;

	private:
		WindowCallbackData m_CallbackData;
	};
}
#endif // HIGHLO_API_DX11
