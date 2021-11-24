// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsWindow.h"

#ifndef HIGHLO_API_GLFW

#include <windowsx.h>

#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Context.h"
#elif HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLContext.h"
#endif // HIGHLO_API_DX11

#include "Engine/Events/Events.h"
#include "Engine/Core/Defines/KeyCodes.h"
#include "Engine/Core/Defines/MouseButtonCodes.h"
#include "Engine/Utils/InputUtils.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static bool s_IsMenubarSet = false;

namespace highlo
{
	static POINTFLOAT s_PreviousMousePos;
	static bool s_ShouldRegisterMouseMovedCallback = false;
	WindowsWindow::WNDPlacement WindowsWindow::m_Placement;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	WindowsWindow::WindowsWindow(const WindowData &properties)
	{
		Window::s_WindowInstance = this;
		m_Properties = properties;
		Init();
	}

	WindowsWindow::~WindowsWindow()
	{
		if (m_NativeHandle)
			DestroyWindow(m_NativeHandle);
	}

	void WindowsWindow::Init()
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
		wc.hInstance = NULL;
		wc.lpszClassName = L"HLEngineWindowClass";
		wc.lpszMenuName = L"";
		wc.lpfnWndProc = WindowProc;
		wc.style = NULL;

		if (!RegisterClassEx(&wc))
		{
			HL_CORE_ERROR("Failed to register Win32 window class");
		}

		RECT window_rect;
		window_rect.left = 0;
		window_rect.top = 30;
		window_rect.right = window_rect.left + m_Properties.Width;
		window_rect.bottom = window_rect.top + m_Properties.Height;

		AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		HWND hwnd = CreateWindowExA(
			WS_EX_OVERLAPPEDWINDOW, "HLEngineWindowClass", m_Properties.Title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
			window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
			NULL, NULL, NULL, &m_Properties
		);

		if (!hwnd)
		{
			HL_CORE_ERROR("Failed to create window HWND");
		}

		m_NativeHandle = hwnd;

		if (m_Properties.Fullscreen)
		{
			auto WindowHDC = GetDC(hwnd);
			auto FullscreenWidth = GetDeviceCaps(WindowHDC, DESKTOPHORZRES);
			auto FullscreenHeight = GetDeviceCaps(WindowHDC, DESKTOPVERTRES);
			auto ColourBits = GetDeviceCaps(WindowHDC, BITSPIXEL);
			auto RefreshRate = GetDeviceCaps(WindowHDC, VREFRESH);

			SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, FullscreenWidth, FullscreenHeight, SWP_SHOWWINDOW);

			ShowWindow(hwnd, SW_MAXIMIZE);
			UpdateWindow(hwnd);

			m_Properties.Width = FullscreenWidth;
			m_Properties.Height = FullscreenHeight;
		}
		else
		{
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

		m_Context = RenderingContext::Create(m_NativeHandle);
		m_Context->Init();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn &callback)
	{
		m_CallbackData.EventCallback = callback;
		m_CallbackData.p_EngineWindow = this;
		m_Properties.EventCallback = callback;
		SetWindowLongPtr(m_NativeHandle, GWLP_USERDATA, (DWORD_PTR) &m_CallbackData);
	}

	const EventCallbackFn &WindowsWindow::GetEventCallback() const
	{
		return m_Properties.EventCallback;
	}

	void WindowsWindow::SetWindowIcon(const HLString &path, bool flip)
	{
		HANDLE hIcon = LoadImage(0, path.W_Str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (hIcon)
		{
			// Change both icons to the same icon handle.
			SendMessage(m_NativeHandle, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
			SendMessage(m_NativeHandle, WM_SETICON, ICON_BIG, (LPARAM) hIcon);

			// This will ensure that the application icon gets changed too.
			SendMessage(GetWindow(m_NativeHandle, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
			SendMessage(GetWindow(m_NativeHandle, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM) hIcon);
		}
	}

	std::pair<int32, int32> WindowsWindow::GetWindowDimensions()
	{
		HDC windowHDC = GetDC(m_NativeHandle);
		int32 width = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
		int32 height = GetDeviceCaps(windowHDC, DESKTOPVERTRES);
		return { width, height };
	}

	std::pair<int32, int32> WindowsWindow::GetWindowPosition()
	{
		RECT windowRect;
		GetWindowRect(m_NativeHandle, &windowRect);
		return { windowRect.left, windowRect.top };
	}

	void WindowsWindow::CloseWindow()
	{
		WindowCloseEvent event;
		m_Properties.EventCallback(event);
		PostQuitMessage(0);
	}

	int32 WindowsWindow::ShowMessageBox(const HLString &title, const HLString &msg, WindowMessageButtonType btnType, WindowMessageIcon icon)
	{
		uint32 iconFlags;
		uint32 buttonFlags;

		switch (btnType)
		{
			case WindowMessageButtonType::BTN_ABORT_RETRY_IGNORE:
				iconFlags = MB_ABORTRETRYIGNORE;
				break;

			case WindowMessageButtonType::BTN_CANCEL_TRY_CONTINUE:
				iconFlags = MB_CANCELTRYCONTINUE;
				break;

			case WindowMessageButtonType::BTN_HELP:
				iconFlags = MB_HELP;
				break;

			case WindowMessageButtonType::BTN_OK:
				iconFlags = MB_OK;
				break;

			case WindowMessageButtonType::BTN_OK_CANCEL:
				iconFlags = MB_OKCANCEL;
				break;

			case WindowMessageButtonType::BTN_RETRY_CANCEL:
				iconFlags = MB_RETRYCANCEL;
				break;

			case WindowMessageButtonType::BTN_YES_NO:
				iconFlags = MB_YESNO;
				break;

			case WindowMessageButtonType::BTN_YES_NO_CANCEL:
				iconFlags = MB_YESNOCANCEL;
				break;

			case WindowMessageButtonType::None:
			default:
				iconFlags = MB_YESNO;
				break;
		}

		switch (icon)
		{
			case WindowMessageIcon::ICON_ERROR:
				buttonFlags = MB_ICONERROR;
				break;

			case WindowMessageIcon::ICON_INFORMATION:
				buttonFlags = MB_ICONINFORMATION;
				break;

			case WindowMessageIcon::ICON_QUESTION:
				buttonFlags = MB_ICONQUESTION;
				break;

			case WindowMessageIcon::ICON_WARNING:
				buttonFlags = MB_ICONWARNING;
				break;

			case WindowMessageIcon::None:
			default:
				buttonFlags = MB_ICONINFORMATION;
				break;
		}

		return MessageBoxW(m_NativeHandle, msg.W_Str(), title.W_Str(), iconFlags | buttonFlags);
	}

	void WindowsWindow::SetMenuBar(const Ref<MenuBar> &bar)
	{
		if (s_IsMenubarSet)
			return;

		s_IsMenubarSet = true;
		m_MenuBar = bar;
		SetMenu(m_NativeHandle, (HMENU) m_MenuBar->GetNativeMenuBar());
	}

	void WindowsWindow::SetVSync(bool bEnabled)
	{
		m_Properties.VSync = bEnabled;
		m_Context->SetSwapInterval(bEnabled);
	}

	void WindowsWindow::SetVisible(bool bVisible)
	{
		m_Properties.Visible = bVisible;
		ShowWindow(m_NativeHandle, bVisible);
	}

	void WindowsWindow::SetFocus(bool bEnabled)
	{
		m_Properties.Focused = bEnabled;
		if (bEnabled)
			::SetFocus(m_NativeHandle);
		else
			::ShowWindow(m_NativeHandle, SW_MINIMIZE);
	}

	void WindowsWindow::SetFullscreen(bool bEnabled)
	{
		m_Properties.Fullscreen = bEnabled;
		if (bEnabled)
		{
			auto [width, height] = GetWindowDimensions();

			GetWindowPlacement(m_NativeHandle, &m_Placement.Placement);
			m_Placement.WindowWidth = width;
			m_Placement.WindowHeight = height;

			SetWindowLongPtrW(m_NativeHandle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtrW(m_NativeHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(m_NativeHandle, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);

			ShowWindow(m_NativeHandle, SW_MAXIMIZE);
			UpdateWindow(m_NativeHandle);
		}
		else
		{
			SetWindowPlacement(m_NativeHandle, &m_Placement.Placement);
			SetWindowLongPtrW(m_NativeHandle, GWL_EXSTYLE, WS_EX_OVERLAPPEDWINDOW);
			SetWindowLongPtrW(m_NativeHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);

			ShowWindow(m_NativeHandle, true);
			UpdateWindow(m_NativeHandle);
		}
	}

	void WindowsWindow::ShowCursor()
	{
		m_Properties.CursorVisible = true;
		::SetCursor(m_Cursor);
		::ShowCursor(true);
	}

	void WindowsWindow::HideCursor()
	{
		m_Properties.CursorVisible = false;
		::SetCursor(NULL);
		::ShowCursor(false);
	}

	void WindowsWindow::Maximize()
	{
		m_Properties.Maximized = true;
		ShowWindow(m_NativeHandle, SW_MAXIMIZE);
	}

	void WindowsWindow::CenterWindow()
	{
		m_Properties.Centered = true;

		RECT windowRect;
		GetWindowRect(m_NativeHandle, &windowRect);

		int32 xPos = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		int32 yPos = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(m_NativeHandle, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void WindowsWindow::SetTitle(const HLString &title)
	{
		m_Properties.Title = title;
		SetWindowTextW(m_NativeHandle, title.W_Str());
	}

	bool WindowsWindow::HasMenuBar()
	{
		return m_MenuBar != nullptr;
	}

	bool WindowsWindow::IsFocused()
	{
		return (GetForegroundWindow() == m_NativeHandle);
	}

	void WindowsWindow::OnResize(uint32 width, uint32 height)
	{
		m_Properties.Width = width;
		m_Properties.Height = height;
	}

	void WindowsWindow::Update()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (PeekMessage(&msg, m_NativeHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_NULL)
		{
			if (!IsWindow(m_NativeHandle))
			{
				m_NativeHandle = NULL;
				return;
			}
		}

		m_Context->SwapBuffers();
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		switch (uMsg)
		{
			case WM_CLOSE:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					WindowCloseEvent event;
					data->EventCallback(event);
				}

				DestroyWindow(hwnd);
				break;
			}

			case WM_SIZE:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					WindowResizeEvent event((unsigned int) LOWORD(lParam), (unsigned int) HIWORD(lParam));
					data->EventCallback(event);

					// Updating window object's width and height properties
					data->p_EngineWindow->OnResize((unsigned int) LOWORD(lParam), (unsigned int) HIWORD(lParam));
				}
				break;
			}
				
			case WM_MOUSEMOVE:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					float xPos = (float) GET_X_LPARAM(lParam);
					float yPos = (float) GET_Y_LPARAM(lParam);

					// if cursor is not hidden, simply process the event,
					// otherwise process the logic for the event when cursor is locked
					if (!data->p_EngineWindow->IsCursorHidden())
					{
						float deltaX = xPos - s_PreviousMousePos.x;
						float deltaY = yPos - s_PreviousMousePos.y;

						MouseMovedEvent event(xPos, yPos, deltaX, deltaY);
						data->EventCallback(event);

						s_PreviousMousePos.x = xPos;
						s_PreviousMousePos.y = yPos;

						break;
					}

					if (s_ShouldRegisterMouseMovedCallback)
					{
						float deltaX = xPos - s_PreviousMousePos.x;
						float deltaY = yPos - s_PreviousMousePos.y;

						MouseMovedEvent event(xPos, yPos, deltaX, deltaY);
						data->EventCallback(event);

						s_PreviousMousePos.x = xPos;
						s_PreviousMousePos.y = yPos;

						s_ShouldRegisterMouseMovedCallback = false;

						RECT wRect;
						GetWindowRect((HWND) data->p_EngineWindow->GetNativeHandle(), &wRect);
						SetCursorPos(wRect.left + data->p_EngineWindow->GetWidth() / 2, wRect.top + data->p_EngineWindow->GetHeight() / 2);
					}
					else
					{
						s_ShouldRegisterMouseMovedCallback = true;

						s_PreviousMousePos.x = xPos;
						s_PreviousMousePos.y = yPos;
					}
				}
				break;
			}

			case WM_MOUSEWHEEL:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseScrolledEvent event(0, (float) ((short) HIWORD(wParam)) / (float) WHEEL_DELTA);
					data->EventCallback(event);
				}
				break;
			}

			case WM_LBUTTONDOWN:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonPressedEvent event(HL_MOUSE_BUTTON_LEFT);
					data->EventCallback(event);
				}
				break;
			}
				
			case WM_RBUTTONDOWN:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonPressedEvent event(HL_MOUSE_BUTTON_RIGHT);
					data->EventCallback(event);
				}
				break;
			}
				
			case WM_MBUTTONDOWN:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonPressedEvent event(HL_MOUSE_BUTTON_MIDDLE);
					data->EventCallback(event);
				}
				break;
			}
		
			case WM_LBUTTONUP:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonReleasedEvent event(HL_MOUSE_BUTTON_LEFT);
					data->EventCallback(event);
				}
				break;
			}
				
			case WM_RBUTTONUP:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonReleasedEvent event(HL_MOUSE_BUTTON_RIGHT);
					data->EventCallback(event);
				}
				break;
			}
			
			case WM_MBUTTONUP:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					MouseButtonReleasedEvent event(HL_MOUSE_BUTTON_MIDDLE);
					data->EventCallback(event);
				}
				break;
			}
				
			case WM_KEYDOWN:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					int repeat = (int) (HIWORD(lParam) & KF_REPEAT) ? 1 : 0;
					KeyPressedEvent event(utils::InputHelperWindowsKeyCodeToHL((int) wParam), repeat);
					data->EventCallback(event);
				}
				break;
			}
				
			case WM_KEYUP:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					KeyReleasedEvent event(utils::InputHelperWindowsKeyCodeToHL((int) wParam));
					data->EventCallback(event);
				}
				break;
			}

			case WM_COMMAND:
			{
				WindowCallbackData *data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (data)
				{
					WORD eventID = LOWORD(wParam);
					FileMenuEvent event(eventID);
					data->EventCallback(event);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

#endif // HIGHLO_API_GLFW