// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include "Engine/Window/PopupMenu.h"

namespace highlo
{
	class WindowsPopupMenu : public PopupMenu
	{
	public:

		WindowsPopupMenu(const HLString &name);
		virtual ~WindowsPopupMenu();

		virtual void AddMenuItem(const Ref<PopupMenuItem> &item) override;
		virtual void AddMenuItem(const PopupMenuItem &item) override;
		virtual void AddMenuItem(const HLString &name, int32 id, PopupItemCallback callback, bool visible = true) override;
		virtual void AddSeparator() override;
		virtual void AddSubMenu(const Ref<PopupMenu> &menu) override;

		virtual void Update() const override;
		virtual void OnEvent(Event &e) override;

		virtual std::vector<PopupMenuItem> &GetMenuItems() override { return m_MenuItems; }
		virtual const std::vector<PopupMenuItem> &GetMenuItems() const override { return m_MenuItems; }

		virtual void *GetPopupMenuHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetPopupMenuHandle() const override { return (void*)m_NativeHandle; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		bool OnFileMenuClickedEvent(const FileMenuEvent &e);

		std::vector<PopupMenuItem> m_MenuItems;
		HMENU m_NativeHandle;
		HLString m_Name;
	};
}

#endif // HL_PLATFORM_WINDOWS
