#pragma once

#include "engine/logging/Logger.h"
#include "engine/time/Time.h"
#include "engine/window/Window.h"
#include "engine/events/Events.h"
#include "engine/rendering/Renderer.h"

namespace highlo
{
	struct ApplicationStartupSettings
	{
		bool Fullscreen = false;
		uint32_t WindowWidth = 1270;
		uint32_t WindowHeight = 920;
		std::string WindowTitle = "HighLo Engine";
	};

	class HLApplication
	{
	public:
		HLAPI HLApplication& Get() { return *s_Instance; }

		HLAPI HLApplication();
		HLAPI ~HLApplication() = default;

		HLAPI void Run();

		HLAPI virtual void OnInitialize() {}
		HLAPI virtual void OnUpdate(Timestep timestep) {}
		HLAPI virtual void OnShutdown() {}
		HLAPI virtual void OnEvent(Event& e) {}

		inline void Close() { m_Running = false; }
		HLAPI void SetApplicationStartupSettings(const ApplicationStartupSettings& settings) { m_StartupSettings = settings; }

	private:
		static HLApplication* s_Instance;
		ApplicationStartupSettings m_StartupSettings;

		bool m_Running = false;

		UniqueRef<Window> m_Window;
		void InitializeWindow();

		bool OnWindowClose(WindowCloseEvent &event);
		bool OnWindowReisze(WindowResizeEvent &event);

	private:
		void InternalEventHandler(Event& event);
	};
}
