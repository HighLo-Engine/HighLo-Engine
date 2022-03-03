// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Application.h"

#include "Engine/Core/VirtualFileSystem.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Service.h"
#include "Engine/Core/Profiler/ProfilerTimer.h"

#include "Engine/Graphics/Framebuffer.h"
#include "Engine/Graphics/Shaders/ShaderCache.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/FontManager.h"

#include "Engine/Math/Math.h"
#include "Engine/ImGui/ImGui.h"
#include "Engine/ECS/RenderSystem.h"
#include "Engine/Threading/ThreadRegistry.h"
#include "Engine/Loaders/AssetImporter.h"

namespace highlo
{
#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&HLApplication::fn, this, std::placeholders::_1)

	HLApplication *HLApplication::s_Instance = nullptr;

	HLApplication::HLApplication()
	{
		Init();
	}

	HLApplication::HLApplication(const ApplicationSettings &settings)
		: m_Settings(settings)
	{
		Init();
	}

	HLApplication::~HLApplication()
	{
		Translations::Shutdown();

		ThreadRegistry::Get()->Shutdown();
		m_Encryptor->Shutdown();
		m_ECS_SystemManager.Shutdown();
		FontManager::Get()->Shutdown();

		// Save the current shader cache state into the json registry
		ShaderCache::Shutdown();

		AssetImporter::Shutdown();

		Renderer::Shutdown();
		Logger::Shutdown();
	}

	void HLApplication::Run()
	{
		m_Running = true;
		m_IsShuttingDown = false;
		OnInitialize();
		Service::OnInit();

		// Main Rendering Thread
		while (m_Running)
		{
			Time::TimeUpdate();

		#ifdef HL_DEBUG
			if (Input::IsKeyPressed(HL_KEY_ESCAPE))
				break;
		#endif

			if (!m_Minimized && !m_Settings.Headless)
			{
				// Update Entities and Client Application
				m_ECS_SystemManager.Update();
				OnUpdate(Time::GetTimestep());

				// Update all layers pushed by the Client Application
				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUpdate(Time::GetTimestep());

				// Update all services
				Service::OnUpdate();

				// Render all submitted objects to the screen
				Renderer::BeginFrame();
				Renderer::WaitAndRender();
				Renderer::EndFrame();
			}

			// Update UI (render this after everything else to render it on top of the actual rendering)
			if (!m_Minimized && !m_Settings.Headless)
			{
				UI::BeginScene();

				OnUIRender(Time::GetTimestep());

				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUIRender(Time::GetTimestep());

				bool showDebugPanel = true;
				m_RenderDebugPanel->OnUIRender(&showDebugPanel);

				UI::EndScene();
			}
			
			// Swap Window Buffers (Double buffer)
			if (!m_Settings.Headless)
				m_Window->Update();

			Time::FrameUpdate();
		}

		OnShutdown();
		Service::OnExit();
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

	void HLApplication::Close()
	{
		m_IsShuttingDown = true;
		m_Running = false;
	}

	Translation *HLApplication::GetActiveTranslation()
	{
		return m_Translations.GetTranslation(m_Settings.ActiveTranslationLanguageCode);
	}

	void HLApplication::Init()
	{
		HL_ASSERT(!s_Instance, "Only one application can be executed at a time!");
		s_Instance = this;
		m_Settings.MainThreadID = Thread::GetCurrentThreadID();

		Logger::Init();

		// Create cache for sin() and cos()
		CreateCacheSin();
		CreateCacheCos();

		// Read the json registry with previous shader cache data
		ShaderCache::Init();

		// Init Window
		if (!m_Settings.Headless)
		{
			WindowData data;
			data.Title = m_Settings.WindowTitle;
			data.Width = m_Settings.WindowWidth;
			data.Height = m_Settings.WindowHeight;
			data.Fullscreen = m_Settings.Fullscreen;
			data.Maximized = m_Settings.Maximized;
			data.VSync = m_Settings.VSync;

			m_Window = Window::Create(data);
			m_Window->SetEventCallback(BIND_APPLICATION_EVENT_FN(InternalEventHandler));
		}

		// Init Renderer
		if (!m_Settings.Headless)
		{
			Renderer::Init(m_Window.Get());
			Renderer::WaitAndRender();
		}

		m_ECS_SystemManager.RegisterSystem<RenderSystem>("RenderSystem");

		AssetImporter::Init();

		// Init Fonts
		FontManager::Get()->Init();

		m_Encryptor = Ref<Encryptor>::Create();
		m_Encryptor->Init();

		ThreadRegistry::Get()->Init();

		// Sort all registered services
		Service::Sort();

		Translations::Init();

		m_RenderDebugPanel = UniqueRef<RenderDebugPanel>::Create();

		HL_CORE_INFO("Engine Initialized");
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
		m_Window->GetSwapChain()->OnResize(width, height);
		
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
