// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Application.h"

#include "Engine/Core/VirtualFileSystem.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/FontManager.h"
#include "Engine/Math/Math.h"

#include "Engine/ImGui/ImGui.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Engine/ECS/RenderSystem.h"

namespace highlo
{
#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&HLApplication::fn, this, std::placeholders::_1)

	HLApplication *HLApplication::s_Instance = nullptr;

	HLApplication::HLApplication()
	{
		HL_ASSERT(!s_Instance, "Only one application can be executed at a time!");
		s_Instance = this;

		Logger::Init();
		VirtualFileSystem::Init();

		// Create cache for sin() and cos()
		CreateCacheSin();
		CreateCacheCos();

		// Init Window
		m_Window = UniqueRef<Window>(Window::Create(WindowData(
			m_Settings.Fullscreen,
			m_Settings.WindowWidth,
			m_Settings.WindowHeight,
			m_Settings.WindowTitle))
		);

		m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));

		// Init Renderer
		Renderer::Init(m_Window.get());
		m_ECS_SystemManager.RegisterSystem<RenderSystem>("RenderSystem");

		m_Encryptor = Ref<Encryptor>::Create();
		m_Encryptor->Init();

		FontManager::Get()->Init();

		HL_CORE_INFO("Engine Initialized");
	}

	HLApplication::~HLApplication()
	{
		FontManager::Get()->Shutdown();
		m_Encryptor->Shutdown();
		m_ECS_SystemManager.Shutdown();
		Renderer::Shutdown();

		VirtualFileSystem::Shutdown();
		Logger::Shutdown();
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
				m_ECS_SystemManager.Update();
				OnUpdate(Time::GetTimestep());

				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUpdate(Time::GetTimestep());
			}

			UI::BeginScene();
			
			if (!m_Minimized)
			{
				OnUIRender(Time::GetTimestep());

				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUIRender(Time::GetTimestep());
			}
			
			UI::EndScene();
			m_Window->Update();
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

	bool HLApplication::OnWindowClose(WindowCloseEvent &e)
	{
		Close();
		return true;
	}

	bool HLApplication::OnWindowReisze(WindowResizeEvent &e)
	{
		uint32 width = e.GetWidth();
		uint32 height = e.GetHeight();

		if (width == 0 || height == 0)
		{
			// Window is minimized
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::SetViewport(0, 0, width, height);
		
		OnResize(width, height);

		for (ApplicationLayer *layer : m_LayerStack)
			layer->OnResize(width, height);

		auto &fbs = FramebufferPool::GetGlobal()->GetAll();
		for (auto &fb : fbs)
		{
			if (!fb->GetSpecification().NoResize)
				fb->Resize(width, height);
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
