// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsFileMenu.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	WindowsFileMenu::WindowsFileMenu(const HLString &name)
		: m_Name(name)
	{
	#ifndef HIGHLO_API_GLFW
		m_NativeHandle = CreatePopupMenu();
	#endif // HIGHLO_API_GLFW
	}
	
	WindowsFileMenu::~WindowsFileMenu()
	{
	#ifndef HIGHLO_API_GLFW
		DestroyMenu(m_NativeHandle);
	#endif // HIGHLO_API_GLFW
	}

	void WindowsFileMenu::AddMenuItem(const Ref<MenuItem> &item)
	{
	#ifndef HIGHLO_API_GLFW
		if (!m_NativeHandle)
			return;

		HLString name = "";
		if (!item->Shortcut.IsEmpty())
			name = item->Name + "\t" + item->Shortcut;
		else
			name = item->Name;

		if (item->Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item->ID, name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item->ID, name.W_Str());

	#endif // HIGHLO_API_GLFW

		m_MenuItems.push_back(item);
	}
	
	void WindowsFileMenu::AddMenuItem(const HLString &name, const HLString &shortcut, int32 id, const MenuItemCallback &callback, bool visible)
	{
	#ifndef HIGHLO_API_GLFW
		if (!m_NativeHandle)
			return;

		HLString realName = "";
		if (!shortcut.IsEmpty())
			realName = name + "\t" + shortcut;
		else
			realName = name;

		if (visible)
			AppendMenuW(m_NativeHandle, MF_STRING, id, realName.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, id, realName.W_Str());

	#endif // HIGHLO_API_GLFW

		Ref<MenuItem> item = Ref<MenuItem>::Create(name, id, callback, visible, false);
		m_MenuItems.push_back(item);
	}
	
	void WindowsFileMenu::AddSubMenu(const Ref<FileMenu> &other)
	{
	#ifndef HIGHLO_API_GLFW
		HMENU menuItem = (HMENU)other->GetMenuHandle();
		AppendMenuW(m_NativeHandle, MF_POPUP, (UINT_PTR)menuItem, other->GetName().W_Str());
	#endif // HIGHLO_API_GLFW

		Ref<MenuItem> item = Ref<MenuItem>::Create(other->GetName(), -1, nullptr, true, true);
		item->IsSubmenu = true;
		item->SubmenuItems = other->GetMenuItems();
		m_MenuItems.push_back(item);
	}
	
	void WindowsFileMenu::AddSeparator()
	{
	#ifndef HIGHLO_API_GLFW
		AppendMenuW(m_NativeHandle, MF_SEPARATOR, 0, NULL);
	#endif // HIGHLO_API_GLFW

		Ref<MenuItem> item = Ref<MenuItem>::Create("", -1, nullptr, true, true);
		m_MenuItems.push_back(item);
	}

	bool WindowsFileMenu::EnableMenuItem(int32 id, bool bEnabled)
	{
		bool itemFound = false;
		for (int32 i = 0; i < m_MenuItems.size(); ++i)
		{
			if (id == m_MenuItems[i]->ID)
			{
				m_MenuItems[i]->Visible = bEnabled;
				itemFound = true;
				break;
			}
		}

	#ifndef HIGHLO_API_GLFW
		if (bEnabled)
			return ::EnableMenuItem(m_NativeHandle, id, MF_ENABLED);
		else
			return ::EnableMenuItem(m_NativeHandle, id, MF_GRAYED);
	#else
		return itemFound;
	#endif // HIGHLO_API_GLFW
	}
	
	bool WindowsFileMenu::SetCheckmark(int32 id, bool bEnabled)
	{
		bool itemFound = false;
		for (int32 i = 0; i < m_MenuItems.size(); ++i)
		{
			if (m_MenuItems[i]->ID == id)
			{
				itemFound = true;
				m_MenuItems[i]->IsSelected = bEnabled;
				break;
			}
		}

	#ifndef HIGHLO_API_GLFW

		int32 result = -1;

		if (bEnabled)
			result = ::CheckMenuItem(m_NativeHandle, id, MF_CHECKED);
		else
			result = ::CheckMenuItem(m_NativeHandle, id, MF_UNCHECKED);

		if (result == -1)
		{
			HL_CORE_ERROR("Did not find the Menu Item with the ID {0}!", id);
			HL_ASSERT(false);
			return false;
		}

		return result == MF_CHECKED;
	#else
		return itemFound;
	#endif // HIGHLO_API_GLFW
	}
}

#endif // HL_PLATFORM_WINDOWS

