// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <Windows.h>
#include "Engine/Window/Window.h"
#include "Engine/Renderer/RenderingContext.h"

#ifndef HIGHLO_API_GLFW

namespace highlo
{
	class WindowsWindow;

	struct WindowCallbackData
	{
		WindowsWindow *p_EngineWindow = nullptr;
		EventCallbackFn EventCallback;
	};

	class WindowsWindow : public Window
	{
	public:

		WindowsWindow(const WindowData &properties);
		~WindowsWindow();

		virtual void SetEventCallback(const EventCallbackFn &callback) override;
		virtual const EventCallbackFn &GetEventCallback() const override;

		virtual void Update() override;

		virtual uint32 GetWidth() override { return m_Properties.Width; }
		virtual uint32 GetHeight() override { return m_Properties.Height; }
		virtual void *GetNativeHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetNativeContext() override { return (void*)m_Context->GetCurrentContext(); }

		virtual void SetWindowIcon(const HLString &path, bool flip = false) override;
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

		virtual bool IsVisible() override { return m_Properties.Visible; }
		virtual bool IsCursorHidden() override { return !m_Properties.CursorVisible; }
		virtual bool IsMaximized() override { return m_Properties.Maximized; }
		virtual bool IsFullscreen() override { return m_Properties.Fullscreen; }
		virtual bool IsCentered() override { return m_Properties.Centered; }
		virtual bool HasMenuBar() override;
		virtual const HLString &GetTitle() override { return m_Properties.Title; }
		virtual const Ref<MenuBar> &GetMenuBar() const override { return m_MenuBar; }
		bool IsFocused() override;

		// Specific to DX11Window
		void OnResize(uint32 width, uint32 height);

	private:

		struct WNDPlacement
		{
			int32 WindowWidth = 0, WindowHeight = 0;
			WINDOWPLACEMENT Placement;
		};

		WindowData m_Properties;
		static WNDPlacement m_Placement;

		void Init();

		UniqueRef<RenderingContext> m_Context;

		HWND m_NativeHandle = nullptr;
		HICON m_WindowIcon = NULL;
		HCURSOR m_Cursor = NULL;
		Ref<MenuBar> m_MenuBar = nullptr;
		bool m_CursorLocked = false;

		WindowCallbackData m_CallbackData;
	};
}

#endif // HIGHLO_API_GLFW