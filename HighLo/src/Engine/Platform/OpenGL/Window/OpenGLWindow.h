#pragma once

#include "Engine/Window/Window.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_OPENGL
#include <GLFW/glfw3.h>

#include "Engine/Renderer/RenderingContext.h"

namespace highlo
{
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowData& properties);
		~OpenGLWindow();

		virtual void SetEventCallback(const EventCallbackFn& callback) override;

		virtual void Update() override;

		virtual unsigned int GetWidth() override;
		virtual unsigned int GetHeight() override;
		virtual void* GetNativeHandle() override { return (void*)m_NativeHandle; }

		virtual void SetCursorLocked(bool bLocked) override;
		bool IsCursorLocked() override;

		bool IsFocused() override;

	private:
		WindowData m_Properties;
		void Init();

		UniqueRef<RenderingContext> m_Context;

	private:
		GLFWwindow* m_NativeHandle = nullptr;
		bool m_CursorLocked = false;

	private:
		void SetCallbacks();
	};
}
#endif // HIGHLO_API_OPENGL
