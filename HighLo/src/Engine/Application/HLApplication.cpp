#include "HighLoPch.h"
#include "HLApplication.h"

#include "Engine/Core/HLVirtualFileSystem.h"
#include "Engine/Core/HLInput.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Math/HLMath.h"

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

		InitializeWindow();
		Renderer::Init(m_Window.get());
		Create_ECS_Systems();

		HL_CORE_INFO("Engine Initialized");
	}

	HLApplication::~HLApplication()
	{
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

			OnUpdate(Time::GetTimestep());
			m_ECS_SystemManager.Update();

			ImGuiRenderer::StartScene();
			OnUIRender(Time::GetTimestep());
			ImGuiRenderer::EndScene();

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
			m_StartupSettings.WindowTitle))
		);

		m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));
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

		Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
		OnResize(event.GetWidth(), event.GetHeight());

		auto &fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto &fb : fbs)
		{
			if (!fb->GetSpecification().NoResize)
				fb->Resize(event.GetWidth(), event.GetHeight());
		}

		return true;
	}

	void HLApplication::Create_ECS_Systems()
	{
		m_ECS_SystemManager.RegisterSystem<RenderSystem>("RenderSystem");
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
