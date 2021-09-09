#include "HighLoPch.h"
#include "Application.h"

#include "Engine/Core/VirtualFileSystem.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Math/Math.h"

#include "Engine/ImGui/ImGui.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Engine/ECS/RenderSystem.h"

namespace highlo
{
#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&HLApplication::fn, this, std::placeholders::_1)

	HLApplication* HLApplication::s_Instance = nullptr;

	HLApplication::HLApplication()
	{
		s_Instance = this;

		Logger::Init();
		VirtualFileSystem::Init();

		// Create cache for sin() and cos()
		CreateCacheSin();
		CreateCacheCos();

		// Init Window
		m_Window = UniqueRef<Window>(Window::Create(WindowData(
			m_StartupSettings.Fullscreen,
			m_StartupSettings.WindowWidth,
			m_StartupSettings.WindowHeight,
			m_StartupSettings.WindowTitle))
			);

		m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));

		// Init Renderer
		Renderer::Init(m_Window.get());
		m_ECS_SystemManager.RegisterSystem<RenderSystem>("RenderSystem");

		m_Encryptor = Ref<Encryptor>::Create();
		m_Encryptor->Init();

		HL_CORE_INFO("Engine Initialized");

	//	HL_PROFILE_BEGIN_SESSION("GlobalSession", "performance.json");
	}

	HLApplication::~HLApplication()
	{
	//	HL_PROFILE_END_SESSION();

		m_Encryptor->Shutdown();
		m_ECS_SystemManager.Shutdown();
		Renderer::Shutdown();
		VirtualFileSystem::Shutdown();
	}

	void HLApplication::Run()
	{
		m_Running = true;
		OnInitialize();

		// Main Rendering Thread
		while (m_Running)
		{
			Time::FrameUpdate();

		#ifdef HL_DEBUG
			if (Input::IsKeyPressed(HL_KEY_ESCAPE))
				break;
		#endif

			if (!m_Minimized)
			{
				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUpdate(Time::GetTimestep());

				OnUpdate(Time::GetTimestep());
				m_ECS_SystemManager.Update();
			}

			UI::BeginScene();
			
			if (!m_Minimized)
			{
				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUIRender(Time::GetTimestep());

				OnUIRender(Time::GetTimestep());
			}
			
			m_Window->Update();
			UI::EndScene();
		}

		OnShutdown();
	}

	void HLApplication::PushLayer(ApplicationLayer *layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void HLApplication::PushOverlay(ApplicationLayer *overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
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
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
		
		for (ApplicationLayer *layer : m_LayerStack)
			layer->OnResize(event.GetWidth(), event.GetHeight());

		OnResize(event.GetWidth(), event.GetHeight());

		auto &fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto &fb : fbs)
		{
			if (!fb->GetSpecification().NoResize)
				fb->Resize(event.GetWidth(), event.GetHeight());
		}

		return true;
	}

	void HLApplication::InternalEventHandler(Event &e)
	{
		// Drop certain input events if the window is not in focus
		if (!m_Window->IsFocused() && e.IsInCategory(EventCategory::EventCategoryInput))
			return;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APPLICATION_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_APPLICATION_EVENT_FN(OnWindowReisze));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.m_Handled)
				break;

			(*it)->OnEvent(e);
		}

		OnEvent(e);
	}
}
