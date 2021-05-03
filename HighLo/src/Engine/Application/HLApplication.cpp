#include "HighLoPch.h"
#include "HLApplication.h"

#include "Engine/Core/HLVirtualFileSystem.h"
#include "Engine/Core/HLInput.h"
#include "Engine/Renderer/Framebuffer.h"

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
		VirtualFileSystem::Init();

		InitializeWindow();

		HL_CORE_INFO("Engine Initialized");
		OnInitialize();

		// Main Rendering Thread
		while (m_Running)
		{
			Time::FrameUpdate();

		#ifdef HL_DEBUG
			if (Input::IsKeyPressed(HL_KEY_ESCAPE))
				break;
		#endif

			OnUpdate(Time::GetTimestep());

			m_Window->Update();
		}

		OnShutdown();

		VirtualFileSystem::Shutdown();
	}

	void HLApplication::InitializeWindow()
	{
		m_Window = UniqueRef<Window>(Window::Create(WindowData(
			m_StartupSettings.Fullscreen,
			m_StartupSettings.WindowWidth,
			m_StartupSettings.WindowHeight,
			m_StartupSettings.WindowTitle))
		);

		m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));
		m_Window->CenterWindow();
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

		auto &fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto &fb : fbs)
		{
			if (!fb->GetSpecification().NoResize)
				fb->Resize(event.GetWidth(), event.GetHeight());
		}

		return true;
	}

	void HLApplication::InternalEventHandler(Event &event)
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
