#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Core/dataTypes/HLString.h"
#include "MenuBar.h"

namespace highlo
{
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		uint32 m_Width;
		uint32 m_Height;
		HLString m_Title;
		bool m_Fullscreen = false;
		bool m_Maximized = false;
		bool m_Focused = false;
		bool m_CursorVisible = true;
		bool m_Centered = false;
		bool m_VSync = true;
		bool m_Visible = true;
		EventCallbackFn m_EventCallback = nullptr;

		WindowData(bool fullscreen = false, uint32 width = 1270, uint32 height = 720, const HLString &title = "HLEngine")
			: m_Fullscreen(fullscreen), m_Width(width), m_Height(height), m_Title(title) {}
	};

	enum class WindowMessageButtonType
	{
		None = 0,
		BTN_ABORT_RETRY_IGNORE,
		BTN_CANCEL_TRY_CONTINUE,
		BTN_HELP,
		BTN_OK,
		BTN_OK_CANCEL,
		BTN_RETRY_CANCEL,
		BTN_YES_NO,
		BTN_YES_NO_CANCEL
	};

	enum class WindowMessageIcon
	{
		None = 0,
		ICON_WARNING,
		ICON_QUESTION,
		ICON_INFORMATION,
		ICON_ERROR
	};

	class Window
	{
	public:
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Window& Get();

		static Window* Create(const WindowData& properties = WindowData());
		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual void SetWindowIcon(const HLString &path) = 0;
		virtual std::pair<int32, int32> GetWindowDimensions() = 0;
		virtual std::pair<int32, int32> GetWindowPosition() = 0;
		virtual void CloseWindow() = 0;

		virtual int32 ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType = WindowMessageButtonType::None, WindowMessageIcon icon = WindowMessageIcon::None) = 0;
		virtual void SetMenuBar(const Ref<MenuBar> &bar) = 0;

		virtual void SetVSync(bool bEnabled) = 0;
		virtual void SetVisible(bool bVisible) = 0;
		virtual void SetFocus(bool bEnabled) = 0;
		virtual void SetFullscreen(bool bEnabled) = 0;
		virtual void ShowCursor() = 0;
		virtual void HideCursor() = 0;
		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;
		virtual void SetTitle(const HLString &title) = 0;
		
		virtual bool IsVisible() = 0;
		virtual bool IsCursorHidden() = 0;
		virtual bool IsMaximized() = 0;
		virtual bool IsFullscreen() = 0;
		virtual bool IsCentered() = 0;
		virtual bool IsFocused() = 0;
		virtual const HLString &GetTitle() = 0;

		virtual uint32 GetWidth() = 0;
		virtual uint32 GetHeight() = 0;
		virtual void* GetNativeHandle() = 0;

	protected:

		static Window* s_WindowInstance;
	};
}
