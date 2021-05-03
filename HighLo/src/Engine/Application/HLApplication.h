#pragma once

#include "Engine/Core/HLLog.h"
#include "Engine/Core/HLTime.h"
#include "Engine/Window/Window.h"
#include "Engine/Events/Events.h"
#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	struct ApplicationStartupSettings
	{
		bool Fullscreen = false;
		uint32 WindowWidth = 1270;
		uint32 WindowHeight = 920;
		HLString WindowTitle = "HighLo Engine";
	};

	class HLApplication
	{
	public:
		HLAPI static HLApplication& Get() { return *s_Instance; }
		HLAPI Window &GetWindow() { return *m_Window; }

		HLAPI HLApplication();
		HLAPI ~HLApplication() = default;

		HLAPI void Run();

		HLAPI virtual void OnInitialize() {}
		HLAPI virtual void OnUpdate(Timestep timestep) {}
		HLAPI virtual void OnShutdown() {}
		HLAPI virtual void OnEvent(Event &e) {}

		inline void Close() { m_Running = false; }
		HLAPI void SetApplicationStartupSettings(const ApplicationStartupSettings &settings) { m_StartupSettings = settings; }

	private:
		static HLApplication* s_Instance;
		ApplicationStartupSettings m_StartupSettings;

		bool m_Running = false;

		UniqueRef<Window> m_Window;
		void InitializeWindow();

		bool OnWindowClose(WindowCloseEvent &event);
		bool OnWindowReisze(WindowResizeEvent &event);

	private:
		void InternalEventHandler(Event &event);
	};
}
