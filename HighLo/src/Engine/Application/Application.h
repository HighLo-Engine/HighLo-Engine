// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <queue>

#include "Engine/Core/Log.h"
#include "Engine/Core/Time.h"
#include "ApplicationLayerStack.h"
#include "Engine/Window/Window.h"
#include "Engine/Events/Events.h"
#include "Engine/ECS/ECS_Registry.h"
#include "Engine/ECS/ECS_SystemManager.h"
#include "Engine/Encryption/Encryptor.h"
#include "Engine/Localization/Translations.h"
#include "Engine/Editor/RenderDebugPanel.h"

#include "ApplicationSettings.h"

namespace highlo
{
	class HLApplication
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		HLAPI HLApplication();
		HLAPI HLApplication(const ApplicationSettings &settings);
		HLAPI ~HLApplication();

		HLAPI void Run();

		HLAPI void PushLayer(ApplicationLayer *layer);
		HLAPI void PushOverlay(ApplicationLayer *overlay);

		HLAPI virtual void OnInitialize() {}
		HLAPI virtual void OnUpdate(Timestep timestep) {}
		HLAPI virtual void OnShutdown() {}
		HLAPI virtual void OnEvent(Event &e) {}
		HLAPI virtual void OnUIRender(Timestep timestep) {}
		HLAPI virtual void OnResize(uint32 width, uint32 height) {}

		HLAPI void Close();
		HLAPI void SetApplicationSettings(const ApplicationSettings &settings) { m_Settings = settings; }
		HLAPI ApplicationSettings &GetApplicationSettings() { return m_Settings; }
		HLAPI bool IsShuttingDown() const { return m_IsShuttingDown; }

		HLAPI ECS_Registry &GetECSRegistry() { return m_ECS_Registry; }
		HLAPI ECS_SystemManager &GetECSSystemManager() { return m_ECS_SystemManager; }
		HLAPI Ref<Encryptor> &GetEncryptor() { return m_Encryptor; }

		HLAPI static HLApplication &Get() { return *s_Instance; }
		HLAPI Window &GetWindow() { return *m_Window; }
		HLAPI static bool HasStarted() { return (bool)s_Instance; }
		HLAPI Translations &GetTranslations() { return m_Translations; }
		HLAPI Translation *GetActiveTranslation();

		HLAPI void AddEventCallback(const EventCallbackFn &eventCallback) { m_EventCallbacks.push_back(eventCallback); }

		template<typename Func>
		HLAPI void QueueEvent(Func &&func)
		{
			m_EventQueue.push(func);
		}

		/// Creates & Dispatches an event either immediately, or adds it to an event queue which will be proccessed at the end of each frame
		template<typename TEvent, bool DispatchImmediately = false, typename... TEventArgs>
		HLAPI void DispatchEvent(TEventArgs&&... args)
		{
			static_assert(std::is_assignable_v<Event, TEvent>);

			std::shared_ptr<TEvent> event = std::make_shared<TEvent>(std::forward<TEventArgs>(args)...);
			if constexpr (DispatchImmediately)
			{
				OnEvent(*event);
			}
			else
			{
				std::scoped_lock<std::mutex> lock(m_EventQueueMutex);
				m_EventQueue.push([event]() { HLApplication::Get().OnEvent(*event); });
			}
		}


	private:

		void Init();
		void Shutdown();

		bool OnWindowClose(WindowCloseEvent &e);
		bool OnWindowReisze(WindowResizeEvent &e);

		void InternalEventHandler(Event &e);
		void ProcessEvents();

	private:

		static HLApplication *s_Instance;
		ApplicationSettings m_Settings;

		bool m_Running = false;
		bool m_Minimized = false;
		bool m_IsShuttingDown = false;
		UniqueRef<Window> m_Window;
		Ref<Encryptor> m_Encryptor;
		UniqueRef<RenderDebugPanel> m_RenderDebugPanel;
		Translations m_Translations;

		ECS_Registry m_ECS_Registry;
		ECS_SystemManager m_ECS_SystemManager;
		ApplicationLayerStack m_LayerStack;

		std::mutex m_EventQueueMutex;
		std::queue<std::function<void()>> m_EventQueue;
		std::vector<EventCallbackFn> m_EventCallbacks;

		float m_CPUTime = 0.0f;
		float m_LastFrameTime = 0.0f;
		uint32 m_FPS = 0;

		Timestep m_Frametime;
		Timestep m_TimeStep;
	};

	// this will be implemented by the client application
	HLAPI HLApplication *CreateApp(int argc, char *argv[]);
}

