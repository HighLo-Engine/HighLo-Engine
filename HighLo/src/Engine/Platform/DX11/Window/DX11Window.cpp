#include "HighLoPch.h"
#include "DX11Window.h"

#ifdef HIGHLO_API_DX11

#include <windowsx.h>
#include "Engine/Window/Window.h"
#include "Engine/Platform/DX11/Rendering/DX11Context.h"

#include "Engine/Events/Events.h"
#include "Engine/Core/Defines/HLKeyCodes.h"
#include "Engine/Core/Defines/HLMouseButtonCodes.h"
#include "Engine/Utils/InputUtils.h"

namespace highlo
{
	static POINTFLOAT s_PreviousMousePos;
	static bool s_ShouldRegisterMouseMovedCallback = false;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	DX11Window::DX11Window(const WindowData& properties)
	{
		Window::s_WindowInstance = this;
		m_Properties = properties;
		Init();
	}

	DX11Window::~DX11Window()
	{
		if (m_NativeHandle)
			DestroyWindow(m_NativeHandle);
	}

	void DX11Window::Init()
	{
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
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
		window_rect.right = window_rect.left + m_Properties.m_Width;
		window_rect.bottom = window_rect.top + m_Properties.m_Height;

		AdjustWindowRect(&window_rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		HWND hwnd = CreateWindowExA(
			WS_EX_OVERLAPPEDWINDOW, "HLEngineWindowClass", m_Properties.m_Title, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
			window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
			NULL, NULL, NULL, &m_Properties
		);

		if (!hwnd)
		{
			HL_CORE_ERROR("Failed to create window HWND");
		}

		m_NativeHandle = hwnd;

		if (m_Properties.m_Fullscreen)
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

			m_Properties.m_Width = FullscreenWidth;
			m_Properties.m_Height = FullscreenHeight;
		}
		else
		{
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

#pragma warning (push)
#pragma warning (disable: 4302)
#pragma warning (disable: 6387)
		//HANDLE hIcon = LoadImage(0, L"assets\\logo\\InfraRedIcon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		//if (hIcon) {
		//	//Change both icons to the same icon handle.
		//	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		//	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		//	//This will ensure that the application icon gets changed too.
		//	SendMessage(GetWindow(hwnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		//	SendMessage(GetWindow(hwnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		//}
#pragma warning (pop)

		m_Context = UniqueRef<RenderingContext>(new DX11Context(m_Properties, hwnd));
		m_Context->Init();
	}

	void DX11Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_CallbackData.EventCallback = callback;
		m_CallbackData.p_EngineWindow = this;
		SetWindowLongPtr(m_NativeHandle, GWLP_USERDATA, (DWORD_PTR)&m_CallbackData);
	}

	unsigned int DX11Window::GetWidth()
	{
		return m_Properties.m_Width;
	}

	unsigned int DX11Window::GetHeight()
	{
		return m_Properties.m_Height;
	}

	void DX11Window::SetCursorLocked(bool bLocked)
	{
		m_CursorLocked = bLocked;
		ShowCursor(!bLocked);
	}

	bool DX11Window::IsCursorLocked()
	{
		return m_CursorLocked;
	}

	bool DX11Window::IsFocused()
	{
		return (GetFocus() == m_NativeHandle);
	}

	void DX11Window::OnResize(unsigned int width, unsigned int height)
	{
		m_Properties.m_Width = width;
		m_Properties.m_Height = height;
	}

	void DX11Window::Update()
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

#ifdef IFR_VERDEV
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;
#endif // IFR_VERDEV


		switch (uMsg)
		{
		case WM_CLOSE:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				WindowResizeEvent event((unsigned int)LOWORD(lParam), (unsigned int)HIWORD(lParam));
				data->EventCallback(event);

				// Updating window object's width and height properties
				data->p_EngineWindow->OnResize((unsigned int)LOWORD(lParam), (unsigned int)HIWORD(lParam));
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				float xPos = (float)GET_X_LPARAM(lParam);
				float yPos = (float)GET_Y_LPARAM(lParam);

				// if cursor is not locked, simply process the event,
				// otherwise process the logic for the event when cursor is locked
				if (!data->p_EngineWindow->IsCursorLocked())
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
					GetWindowRect((HWND)data->p_EngineWindow->GetNativeHandle(), &wRect);
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
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseScrolledEvent event(0, (float)((short)HIWORD(wParam)) / (float)WHEEL_DELTA);
				data->EventCallback(event);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonPressedEvent event((int32)HLMouseButtonCode::BUTTON_LEFT);
				data->EventCallback(event);
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonPressedEvent event((int32)HLMouseButtonCode::BUTTON_RIGHT);
				data->EventCallback(event);
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonPressedEvent event((int32)HLMouseButtonCode::BUTTON_MIDDLE);
				data->EventCallback(event);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonReleasedEvent event((int32)HLMouseButtonCode::BUTTON_LEFT);
				data->EventCallback(event);
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonReleasedEvent event((int32)HLMouseButtonCode::BUTTON_RIGHT);
				data->EventCallback(event);
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				MouseButtonReleasedEvent event((int32)HLMouseButtonCode::BUTTON_MIDDLE);
				data->EventCallback(event);
			}
			break;
		}
		case WM_KEYDOWN:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				int repeat = (int)(HIWORD(lParam) & KF_REPEAT) ? 1 : 0;
				KeyPressedEvent event(utils::InputHelperWindowsKeyCodeToHL((int)wParam), repeat);
				data->EventCallback(event);
			}
			break;
		}
		case WM_KEYUP:
		{
			WindowCallbackData* data = reinterpret_cast<WindowCallbackData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (data)
			{
				KeyReleasedEvent event(utils::InputHelperWindowsKeyCodeToHL((int) wParam));
				data->EventCallback(event);
			}
			break;
		}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
#endif // HIGHLO_API_DX11
