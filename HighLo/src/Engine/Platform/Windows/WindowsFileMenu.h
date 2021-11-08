// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include "Engine/Window/FileMenu.h"

namespace highlo
{
	class WindowsFileMenu : public FileMenu
	{
	public:

		WindowsFileMenu() = default;
		WindowsFileMenu(const HLString &name);
		virtual ~WindowsFileMenu();

		virtual void AddMenuItem(const Ref<MenuItem> &item) override;
		virtual void AddMenuItem(const HLString &name, const HLString &shortcut, int32 id, MenuItemCallback callback, bool visible = true) override;

		virtual void AddSubMenu(const Ref<FileMenu> &other) override;
		virtual void AddSeparator() override;

		virtual bool EnableMenuItem(int32 id, bool bEnabled) override;
		virtual bool SetCheckmark(int32 id, bool bEnabled) override;

		virtual void *GetMenuHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetMenuHandle() const override { return (void*)m_NativeHandle; }

		virtual std::vector<Ref<MenuItem>> &GetMenuItems() override { return m_MenuItems; }
		virtual const std::vector<Ref<MenuItem>> &GetMenuItems() const override { return m_MenuItems; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }
		
	private:

		HLString m_Name = "DefaultMenuTitle";
		HMENU m_NativeHandle;
		std::vector<Ref<MenuItem>> m_MenuItems;
	};
}

#endif // HL_PLATFORM_WINDOWS