#include "HighLoPch.h"
#include "HLApplication.h"

#include "Engine/Core/DataTypes/HLQueue.h"

namespace highlo
{
#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&HLApplication::fn, this, std::placeholders::_1)

	HLApplication* HLApplication::s_Instance = nullptr;

	HLApplication::HLApplication()
	{
		s_Instance = this;
		test_assimp();
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

	bool HLApplication::OnWindowClose(WindowCloseEvent &event)
		{
		Close();
		return true;
		}

	bool HLApplication::OnWindowReisze(WindowResizeEvent &event)
		{
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
			{
			// Window is minimized
			return false;
			}

		// TODO: Resize Framebuffers and Viewports

		return true;
		}

	void HLApplication::InternalEventHandler(Event& event)
	{
		// Drop certain input events if the window is not in focus
		if (!m_Window->IsFocused() && event.IsInCategory(EventCategory::EventCategoryInput))
			return;

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APPLICATION_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_APPLICATION_EVENT_FN(OnWindowReisze));

		OnEvent(event);
	}
}
