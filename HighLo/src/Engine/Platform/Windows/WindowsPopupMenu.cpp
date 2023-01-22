// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsPopupMenu.h"

#ifdef HL_PLATFORM_WINDOWS

#include "Engine/Core/Input.h"

namespace highlo
{
	static int32 s_LastPosition = 0;

	WindowsPopupMenu::WindowsPopupMenu(const HLString &name)
		: m_Name(name)
	{
		m_NativeHandle = CreatePopupMenu();
	}
	
	WindowsPopupMenu::~WindowsPopupMenu() {}

	void WindowsPopupMenu::AddMenuItem(const Ref<PopupMenuItem> &item)
	{
		++s_LastPosition;
		m_MenuItems.push_back(*item);

		if (item->Visible)
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, item->ID, item->Name.W_Str());
		else
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, item->ID, item->Name.W_Str());
	}

	void WindowsPopupMenu::AddMenuItem(const PopupMenuItem &item)
	{
		++s_LastPosition;
		m_MenuItems.push_back(item);

		if (item.Visible)
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, item.ID, item.Name.W_Str());
		else
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, item.ID, item.Name.W_Str());
	}

	void WindowsPopupMenu::AddMenuItem(const HLString &name, int32 id, PopupItemCallback callback, bool visible)
	{
		++s_LastPosition;
		PopupMenuItem item;
		item.Name = name;
		item.ID = id;
		item.Callback = callback;
		item.Visible = visible;
		item.Separator = false;
		item.IsSubmenu = false;
		m_MenuItems.push_back(item);

		if (visible)
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, id, name.W_Str());
		else
			InsertMenuW(m_NativeHandle, s_LastPosition, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, id, name.W_Str());
	}
	
	void WindowsPopupMenu::AddSeparator()
	{
		++s_LastPosition;
		InsertMenuW(m_NativeHandle, s_LastPosition, MF_SEPARATOR, 0, NULL);

		PopupMenuItem item;
		item.Name = "";
		item.ID = -1;
		item.Callback = nullptr;
		item.Visible = true;
		item.Separator = true;
		item.IsSubmenu = false;
		m_MenuItems.push_back(item);
	}
	
	void WindowsPopupMenu::AddSubMenu(const Ref<PopupMenu> &menu)
	{
		HMENU popupitem = (HMENU)menu->GetPopupMenuHandle();
		++s_LastPosition;
		InsertMenuW(m_NativeHandle, s_LastPosition, MF_STRING | MF_POPUP | MNS_NOTIFYBYPOS, (UINT_PTR)popupitem, menu->GetName().W_Str());

		PopupMenuItem item;
		item.Name = menu->GetName();
		item.ID = -1;
		item.Callback = nullptr;
		item.Visible = true;
		item.Separator = false;
		item.IsSubmenu = true;
		item.SubMenuItems = menu->GetMenuItems();
		m_MenuItems.push_back(item);
	}
	
	void WindowsPopupMenu::Update() const
	{
	#ifdef HIGHLO_API_GLFW
		return; // skip events if GLFW is enabled
	#endif // HIGHLO_API_GLFW

		if (Input::IsMouseButtonPressed(HL_MOUSE_BUTTON_RIGHT))
		{
			HWND window = GetActiveWindow();
			SetForegroundWindow(window);

			POINT p;
			GetCursorPos(&p);

			HL_CORE_TRACE("popupMenu clicked");
			TrackPopupMenuEx(m_NativeHandle, TPM_LEFTBUTTON | TPM_RECURSE, p.x, p.y, window, NULL);
		}
	}

	void WindowsPopupMenu::OnEvent(Event &e)
	{
	#ifdef HIGHLO_API_GLFW
		return; // skip events if GLFW is enabled
	#endif // HIGHLO_API_GLFW

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<FileMenuEvent>(HL_BIND_EVENT_FUNCTION(WindowsPopupMenu::OnFileMenuClickedEvent));
	}

	bool WindowsPopupMenu::OnFileMenuClickedEvent(const FileMenuEvent &e)
	{
		for (int32 i = 0; i < m_MenuItems.size(); ++i)
		{
			PopupMenuItem item = m_MenuItems[i];
			if (item.ID == e.GetID())
			{
				item.Callback(item);
				return true;
			}
		}

		return false;
	}
}

#endif  // HL_PLATFORM_WINDOWS
