#pragma once

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include "Engine/Window/MenuBar.h"

namespace highlo
{
	class WindowsMenuBar : public MenuBar
	{
	public:

		WindowsMenuBar();
		virtual ~WindowsMenuBar();

		virtual void AddMenu(const Ref<FileMenu> &menu) override;
		virtual void RemoveMenu(const Ref<FileMenu> &menu) override;

		virtual void *GetNativeMenuBar() override { return (void*)m_NativeHandle; }
		virtual void *GetNativeMenuBar() const override { return (void*)m_NativeHandle; }

		virtual void OnEvent(Event &e) override;

	private:

		bool OnFileMenuClicked(FileMenuEvent &e);

		HMENU m_NativeHandle;
		std::vector<Ref<FileMenu>> Menus;
	};
}

#endif // HL_PLATFORM_WINDOWS