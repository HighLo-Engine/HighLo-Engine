#pragma once

#ifdef HIGHLO_API_GLFW

#include <GLFW/glfw3.h>

#include "Engine/Window/Window.h"
#include "Engine/Renderer/RenderingContext.h"
#include "Engine/Window/MenuBar.h"

namespace highlo
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowData &properties);
		~GLFWWindow();

		virtual void SetEventCallback(const EventCallbackFn &callback) override;

		virtual void Update() override;

		virtual uint32 GetWidth() override { return m_Properties.m_Width; }
		virtual uint32 GetHeight() override { return m_Properties.m_Height; }
		virtual void *GetNativeHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetNativeContext() override { return (void*) m_Context->GetCurrentContext(); }

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

		virtual bool IsVisible() override { return m_Properties.m_Visible; }
		virtual bool IsCursorHidden() override { return !m_Properties.m_CursorVisible; }
		virtual bool IsMaximized() override { return m_Properties.m_Maximized; }
		virtual bool IsFullscreen() override { return m_Properties.m_Fullscreen; }
		virtual bool IsCentered() override { return m_Properties.m_Centered; }
		virtual const HLString &GetTitle() override { return m_Properties.m_Title; }
		virtual bool HasMenuBar() override { return m_MenuBar != nullptr; }
		virtual const Ref<MenuBar> &GetMenuBar() const override { return m_MenuBar; }
		bool IsFocused() override;

	private:

		struct WNDPlacement
		{
			int32 WindowWidth = 0, WindowHeight = 0;
			int32 WindowPosX = 0, WindowPosY = 0;
		};

		WindowData m_Properties;
		WNDPlacement m_Placement;

		Ref<MenuBar> m_MenuBar = nullptr;
		UniqueRef<RenderingContext> m_Context;

		GLFWwindow *m_NativeHandle = nullptr;
		bool m_CursorLocked = false;

		void SetCallbacks();
		void Init();
	};
}

#endif // HIGHLO_API_GLFW