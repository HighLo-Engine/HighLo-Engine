#include "HLApplication.h"
#include <logging/Logger.h>

namespace highlo
{
#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&HLApplication::fn, this, std::placeholders::_1)

	HLApplication* HLApplication::s_Instance = nullptr;

	HLApplication::HLApplication()
	{
		s_Instance = this;
	}

	void HLApplication::Run()
	{
		Logger::Init();
		m_Running = true;

		InitializeWindow();

		HL_CORE_INFO("Engine Initialized");
		OnInitialize();

		// Main Rendering Thread
		while (m_Running)
		{
			Time::FrameUpdate();

			OnUpdate(Time::GetTimestep());

			m_Window->Update();
		}

		OnShutdown();
	}

	void HLApplication::InitializeWindow()
	{
		m_Window = UniqueRef<Window>(Window::Create(WindowData(
			m_StartupSettings.Fullscreen,
			m_StartupSettings.WindowWidth,
			m_StartupSettings.WindowHeight,
			m_StartupSettings.WindowTitle.c_str()))
		);

		m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));

		Renderer::Init(m_Window.get());
	}

	void HLApplication::InternalEventHandler(Event& event)
	{
		// Drop certain input events if the window is not in focus
		if (!m_Window->IsFocused() && event.IsInCategory(EventCategory::EventCategoryInput))
			return;

		OnEvent(event);

		if (event.GetEventType() == EventType::WindowClose)
		{
			Close();
		}
		else if (event.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& wre = static_cast<WindowResizeEvent&>(event);
		}
	}
}
