#pragma once

#include "engine/core/HLCore.h"
#include "engine/events/Events.h"
#include "engine/core/Log.h"
#include "engine/core/dataTypes/HLString.h"

namespace highlo
{
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		uint32 m_Width;
		uint32 m_Height;
		HLString m_Title;
		bool m_Fullscreen;
		bool m_Focused = false;
		EventCallbackFn m_EventCallback = nullptr;

		WindowData(bool fullscreen = false, uint32 width = 1270, uint32 height = 720, const HLString &title = "HLEngine")
			: m_Fullscreen(fullscreen), m_Width(width), m_Height(height), m_Title(title) {}
	};

	class Window
	{
	public:
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		static Window& Get();

		static Window* Create(const WindowData& properties = WindowData());
		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual void* GetNativeHandle() = 0;

		virtual void SetCursorLocked(bool bLocked) = 0;
		virtual bool IsCursorLocked() = 0;

		virtual bool IsFocused() = 0;

	protected:

		static Window* s_WindowInstance;
	};
}
