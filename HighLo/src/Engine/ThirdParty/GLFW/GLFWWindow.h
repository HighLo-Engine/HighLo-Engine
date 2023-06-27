// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HIGHLO_API_GLFW

#ifdef HL_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <shobjidl_core.h>
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Engine/Window/Window.h"
#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Window/MenuBar.h"

namespace highlo
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowData &properties);
		~GLFWWindow();

		virtual void SetEventCallback(const EventCallbackFn &callback) override;
		virtual const EventCallbackFn &GetEventCallback() const override;

		virtual void Update() override;

		virtual uint32 GetWidth() override { return m_Properties.Width; }
		virtual uint32 GetHeight() override { return m_Properties.Height; }
		virtual void *GetNativeHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetNativeContext() override { return (void*)m_Context->GetCurrentContext(); }
		virtual void *GetNativeCursor() override { return (void*)m_NativeCursor; }

		virtual void SetWindowIcon(const HLString &path, bool flip = false) override;
		virtual std::pair<int32, int32> GetWindowDimensions() override;
		virtual std::pair<int32, int32> GetWindowPosition() override;
		virtual void CloseWindow() override;

		virtual int32 ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType = WindowMessageButtonType::None, WindowMessageIcon icon = WindowMessageIcon::None) override;
		virtual void SetMenuBar(const Ref<MenuBar> &bar) override;
		virtual bool SetProgress(WindowProgressState state) override;
		virtual bool SetProgressValue(uint64 completed, uint64 total) override;

		virtual void SetVSync(bool bEnabled) override;
		virtual void SetVisible(bool bVisible) override;
		virtual void SetFocus(bool bEnabled) override;
		virtual void SetFullscreen(bool bEnabled) override;
		virtual void SetResizable(bool bEnabled) override;
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
		virtual const HLString &GetTitle() override { return m_Properties.Title; }
		virtual bool HasMenuBar() override { return m_MenuBar != nullptr; }
		virtual const Ref<MenuBar> &GetMenuBar() const override { return m_MenuBar; }
		bool IsFocused() override;

		virtual const WindowData &GetProperties() const override { return m_Properties; }

		virtual Ref<RenderingContext> &GetContext() override { return m_Context; }
		virtual const Ref<RenderingContext> &GetContext() const override { return m_Context; }
		virtual Ref<SwapChain> &GetSwapChain() override { return m_SwapChain; }
		virtual const Ref<SwapChain> &GetSwapChain() const override { return m_SwapChain; }

	private:

		struct WNDPlacement
		{
			int32 WindowWidth = 0, WindowHeight = 0;
			int32 WindowPosX = 0, WindowPosY = 0;
		};

		WindowData m_Properties;
		WNDPlacement m_Placement;

#ifdef HL_PLATFORM_WINDOWS
		ITaskbarList4 *m_Taskbar;
#endif

		Ref<MenuBar> m_MenuBar = nullptr;
		Ref<RenderingContext> m_Context = nullptr;
		Ref<SwapChain> m_SwapChain = nullptr;

		GLFWwindow *m_NativeHandle = nullptr;
		GLFWcursor *m_NativeCursor = nullptr;
		bool m_CursorLocked = false;

		void SetCallbacks();
		void Init();
	};
}

#endif // HIGHLO_API_GLFW
