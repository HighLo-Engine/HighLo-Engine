// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Core/FileSystem.h"

#define BIND_APPLICATION_EVENT_FN(fn) std::bind(&highlo::HLApplication::fn, this, std::placeholders::_1)

namespace highlo
{
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
		Shutdown();
	}

	void HLApplication::Run()
	{
		m_Running = true;
		m_IsShuttingDown = false;
		OnInitialize();
		Service::OnInit();

		float initialTime = GetTime();
		float previousTime = initialTime;

		// Main Rendering Thread
		while (m_Running)
		{
			static uint32 frameCounter = 0;

#ifdef HL_DEBUG
			if (Input::IsKeyPressed(HL_KEY_ESCAPE))
				break;
#endif

			if (!m_Minimized && !m_Settings.Headless)
			{
				// Update Entities and Client Application
				m_ECS_SystemManager.Update(m_TimeStep);
				OnUpdate(m_TimeStep);

				// Update all layers pushed by the Client Application
				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep);

				// Update all services
				Service::OnUpdate();

				// Render all submitted objects to the screen
				Renderer::BeginFrame();
				m_Window->GetSwapChain()->BeginFrame();
				Renderer::WaitAndRender();
				Renderer::EndFrame();
			}

			// Update UI (render this after everything else to render it on top of the actual rendering)
			if (!m_Minimized && !m_Settings.Headless)
			{
				UI::BeginScene();

				OnUIRender(m_TimeStep);

				for (ApplicationLayer *layer : m_LayerStack)
					layer->OnUIRender(m_TimeStep);

#if HL_DEBUG
				bool showDebugPanel = true;
#else
				bool showDebugPanel = false;
#endif
				
				m_RenderDebugPanel->OnUIRender(&showDebugPanel, m_Frametime * 1000.0f, m_FPS, m_LastFrameTime);

				UI::EndScene();
			}
			
			// Swap Window Buffers (Double buffer)
			if (!m_Settings.Headless)
				m_Window->Update();

		//	HL_CORE_TRACE("FRAME TIME: {}", m_Frametime * 1000.0f);
			float time = GetTime();
			m_Frametime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_Frametime, 0.0333f);
			m_LastFrameTime = time;

			frameCounter++;

			if (time - previousTime >= 1.0f)
			{
				m_FPS = frameCounter;
				frameCounter = 0;
				previousTime = GetTime();
			}

			m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % Renderer::GetConfig().FramesInFlight;
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

		if (!m_Settings.WorkingDirectory.IsEmpty())
		{
			FileSystem::Get()->SetCurrentWorkingDirectory(m_Settings.WorkingDirectory);
		}

		HL_CORE_TRACE("CWD: {0}", FileSystem::Get()->GetCurrentWorkingDirectory().String().C_Str());

		// Create cache for sin() and cos()
		Math::CreateCacheSin();
		Math::CreateCacheCos();

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

			// Init Renderer
			Renderer::Init(m_Window.Get());
			Renderer::WaitAndRender();
		}

		m_ECS_SystemManager.RegisterSystem<RenderSystem>("RenderSystem");

		AssetImporter::Init();

		// Init Fonts
		FontManager::Get()->Init();

		// The encryption module is used to encrypt engine internal data
		// as well as for multiplayer communications
		m_Encryptor = Ref<Encryptor>::Create();
		m_Encryptor->Init();

		ThreadRegistry::Get()->Init();

		// Sort all registered services
		Service::Sort();

		Translations::Init();

		ScriptEngineConfig scriptConfig;
		ScriptEngine::Init(&scriptConfig);

		m_RenderDebugPanel = UniqueRef<RenderDebugPanel>::Create();

		HL_CORE_INFO("Engine Initialized");
	}

	void HLApplication::Shutdown()
	{
		ScriptEngine::Shutdown();
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

	bool HLApplication::OnWindowClose(WindowCloseEvent &e)
	{
		// Skip window close event in headleass mode
		if (m_Settings.Headless)
			return true;

		Close();
		return true;
	}

	bool HLApplication::OnWindowReisze(WindowResizeEvent &e)
	{
		// Skip window resize events if app is being run from a console
		// (we do not have a window to resize)
		if (m_Settings.Headless)
			return true;

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

		// Notify all attached layers that an event has occurred (back-to-front order)
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.m_Handled)
				break;

			(*it)->OnEvent(e);
		}

		// Notify the main client application of the event
		OnEvent(e);
	}
}
