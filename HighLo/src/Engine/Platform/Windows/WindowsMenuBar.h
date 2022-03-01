// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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

		inline virtual std::vector<Ref<FileMenu>> &GetMenus() override { return m_Menus; }
		inline virtual const std::vector<Ref<FileMenu>> &GetMenus() const override { return m_Menus; }

		virtual void *GetNativeMenuBar() override { return (void*)m_NativeHandle; }
		virtual void *GetNativeMenuBar() const override { return (void*)m_NativeHandle; }

		virtual void OnEvent(Event &e) override;

	private:

		bool OnFileMenuClicked(FileMenuEvent &e);
		bool OnFileMenuChanged(FileMenuChangedEvent &e);
		bool RenderSubMenu(const HLString &name, std::vector<Ref<MenuItem>> &items, Ref<FileMenu> parentMenu, int32 eventID);

		HMENU m_NativeHandle;
		std::vector<Ref<FileMenu>> m_Menus;
	};
}

#endif // HL_PLATFORM_WINDOWS
