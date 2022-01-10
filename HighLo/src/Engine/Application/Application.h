// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Log.h"
#include "Engine/Core/Time.h"
#include "ApplicationLayerStack.h"
#include "Engine/Window/Window.h"
#include "Engine/Events/Events.h"
#include "Engine/ECS/ECS_Registry.h"
#include "Engine/ECS/ECS_SystemManager.h"
#include "Engine/Encryption/Encryptor.h"

#include "ApplicationSettings.h"

namespace highlo
{
	class HLApplication
	{
	public:
		HLAPI static HLApplication &Get() { return *s_Instance; }
		HLAPI Window &GetWindow() { return *m_Window; }

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

	private:
		static HLApplication *s_Instance;
		ApplicationSettings m_Settings;

		bool m_Running = false;
		bool m_Minimized = false;
		bool m_IsShuttingDown = false;
		UniqueRef<Window> m_Window;
		Ref<Encryptor> m_Encryptor;

		ECS_Registry m_ECS_Registry;
		ECS_SystemManager m_ECS_SystemManager;
		ApplicationLayerStack m_LayerStack;

		void Init();
		bool OnWindowClose(WindowCloseEvent &event);
		bool OnWindowReisze(WindowResizeEvent &event);
		bool OnFileMenuChangedEvent(FileMenuChangedEvent &event);
		bool OnFileSystemChangedEvent(FileSystemChangedEvent &event);

	private:

		void InternalEventHandler(Event &event);
	};

	// this will be implemented by the client application
	HLAPI HLApplication *CreateApp(int argc, char *argv[]);
}

