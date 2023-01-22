// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Events.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/UniqueReference.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Graphics/RenderingContext.h"
#include "Engine/Graphics/SwapChain.h"
#include "MenuBar.h"

// Windows macro
#undef ERROR

namespace highlo
{
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		uint32 Width;
		uint32 Height;
		HLString Title;
		bool Fullscreen = false;
		bool Maximized = false;
		bool Focused = false;
		bool CursorVisible = true;
		bool Centered = false;
		bool VSync = true;
		bool Visible = true;
		EventCallbackFn EventCallback = nullptr;

		WindowData(bool fullscreen = false, uint32 width = 1270, uint32 height = 720, const HLString &title = "HLEngine")
			: Fullscreen(fullscreen), Width(width), Height(height), Title(title) {}
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

	enum class WindowProgressState
	{
		None = 0,
		NORMAL,
		ERROR,
		PAUSED,
		NOPROGRESS,
		INDETERMINATE
	};

	class Window
	{
	public:

		HLAPI virtual ~Window() = default;

		HLAPI virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
		HLAPI virtual const EventCallbackFn &GetEventCallback() const = 0;

		HLAPI virtual void Update() = 0;

		HLAPI virtual void SetWindowIcon(const HLString &path, bool flip = false) = 0;
		HLAPI virtual std::pair<int32, int32> GetWindowDimensions() = 0;
		HLAPI virtual std::pair<int32, int32> GetWindowPosition() = 0;
		HLAPI virtual void CloseWindow() = 0;

		HLAPI virtual int32 ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType = WindowMessageButtonType::None, WindowMessageIcon icon = WindowMessageIcon::None) = 0;
		HLAPI virtual void SetMenuBar(const Ref<MenuBar> &bar) = 0;
		HLAPI virtual bool SetProgress(WindowProgressState state) = 0;
		HLAPI virtual bool SetProgressValue(uint64 completed, uint64 total) = 0;

		HLAPI virtual void SetVSync(bool bEnabled) = 0;
		HLAPI virtual void SetVisible(bool bVisible) = 0;
		HLAPI virtual void SetFocus(bool bEnabled) = 0;
		HLAPI virtual void SetFullscreen(bool bEnabled) = 0;
		HLAPI virtual void ShowCursor() = 0;
		HLAPI virtual void HideCursor() = 0;
		HLAPI virtual void Maximize() = 0;
		HLAPI virtual void CenterWindow() = 0;
		HLAPI virtual void SetTitle(const HLString &title) = 0;
		
		HLAPI virtual bool IsVisible() = 0;
		HLAPI virtual bool IsCursorHidden() = 0;
		HLAPI virtual bool IsMaximized() = 0;
		HLAPI virtual bool IsFullscreen() = 0;
		HLAPI virtual bool IsCentered() = 0;
		HLAPI virtual bool IsFocused() = 0;
		HLAPI virtual bool HasMenuBar() = 0;
		HLAPI virtual const HLString &GetTitle() = 0;
		HLAPI virtual const Ref<MenuBar> &GetMenuBar() const = 0;

		HLAPI virtual uint32 GetWidth() = 0;
		HLAPI virtual uint32 GetHeight() = 0;
		HLAPI virtual void *GetNativeHandle() = 0;
		HLAPI virtual void *GetNativeContext() = 0;
		HLAPI virtual void *GetNativeCursor() = 0;

		HLAPI virtual Ref<RenderingContext> &GetContext() = 0;
		HLAPI virtual const Ref<RenderingContext> &GetContext() const = 0;
		HLAPI virtual Ref<SwapChain> &GetSwapChain() = 0;
		HLAPI virtual const Ref<SwapChain> &GetSwapChain() const = 0;
		HLAPI virtual const WindowData &GetProperties() const = 0;

		HLAPI static Window &Get();
		HLAPI static UniqueRef<Window> Create(const WindowData &properties = WindowData());

	protected:

		static Window* s_WindowInstance;
	};
}
