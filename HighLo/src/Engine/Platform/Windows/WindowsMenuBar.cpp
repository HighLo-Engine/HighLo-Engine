// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsMenuBar.h"

#include "Engine/Application/Application.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	WindowsMenuBar::WindowsMenuBar()
	{
		m_NativeHandle = CreateMenu();
	}

	WindowsMenuBar::~WindowsMenuBar()
	{
		DestroyMenu(m_NativeHandle);
	}

	void WindowsMenuBar::AddMenu(const Ref<FileMenu> &menu)
	{
		HMENU menuItem = (HMENU)menu->GetMenuHandle();
		AppendMenuW(m_NativeHandle, MF_POPUP, (UINT_PTR)menuItem, menu->GetName().W_Str());
		m_Menus.push_back(menu);
	}

	void WindowsMenuBar::RemoveMenu(const Ref<FileMenu> &menu)
	{
		DestroyMenu((HMENU)menu->GetMenuHandle());

		auto it = std::find(m_Menus.begin(), m_Menus.end(), menu);
		if (it != m_Menus.end())
			m_Menus.erase(it);
	}

	void WindowsMenuBar::OnEvent(Event &e)
	{
	#ifdef HIGHLO_API_GLFW
		return; // skip events if GLFW is enabled
	#endif // HIGHLO_API_GLFW

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<FileMenuEvent>(HL_BIND_EVENT_FUNCTION(WindowsMenuBar::OnFileMenuClicked));
		dispatcher.Dispatch<FileMenuChangedEvent>(HL_BIND_EVENT_FUNCTION(WindowsMenuBar::OnFileMenuChanged));
	}

	bool WindowsMenuBar::OnFileMenuClicked(FileMenuEvent &e)
	{
		int32 eventID = e.GetID();
		bool result = false;

		for (int32 i = 0; i < m_Menus.size(); ++i)
		{
			std::vector<Ref<MenuItem>> menuItems = m_Menus[i]->GetMenuItems();
			for (int32 j = 0; j < menuItems.size(); ++j)
			{
				Ref<MenuItem> currentItem = menuItems[j];

				if (currentItem->IsSubmenu)
				{
					result = RenderSubMenu(currentItem->Name, currentItem->SubmenuItems, m_Menus[i], eventID);
					continue;
				}

				if (eventID == currentItem->ID)
				{
					Ref<MenuItem> before = Ref<MenuItem>::Create(currentItem->Name, currentItem->ID, currentItem->Callback, currentItem->Visible, currentItem->Separator);
					before->IsSelected = currentItem->IsSelected;
					before->IsSubmenu = currentItem->IsSubmenu;
					before->SubmenuItems = currentItem->SubmenuItems;
					before->Shortcut = currentItem->Shortcut;
					currentItem->Callback(m_Menus[i].Get(), currentItem.Get());
					if (!before.Equals(menuItems[j]))
					{
						FileMenuChangedEvent e(currentItem);
						HLApplication::Get().GetWindow().GetEventCallback()(e);
					}

					result = true;
				}
			}
		}

		return result;
	}

	bool WindowsMenuBar::OnFileMenuChanged(FileMenuChangedEvent &e)
	{
		// Fine this menuItem and change it's properties
		Ref<MenuItem> changedItem = e.GetItem();

		for (uint32 i = 0; i < m_Menus.size(); ++i)
		{
			std::vector<Ref<MenuItem>> menuItems = m_Menus[i]->GetMenuItems();
			for (uint32 j = 0; j < menuItems.size(); ++j)
			{
				if (changedItem->ID == menuItems[j]->ID)
				{
					// We found the menu item, that has been changed!
					menuItems[j]->IsSubmenu = changedItem->IsSubmenu;
					menuItems[j]->Name = changedItem->Name;
					menuItems[j]->Separator = changedItem->Separator;
					menuItems[j]->Shortcut = changedItem->Shortcut;
					menuItems[j]->Visible = changedItem->Visible;
					menuItems[j]->SubmenuItems = changedItem->SubmenuItems;
					menuItems[j]->Callback = changedItem->Callback;
					menuItems[j]->IsSelected = changedItem->IsSelected;
					m_Menus[i]->SetCheckmark(menuItems[j]->ID, menuItems[j]->IsSelected);
					return true;
				}
			}
		}

		return false;
	}
	
	bool WindowsMenuBar::RenderSubMenu(const HLString &name, std::vector<Ref<MenuItem>> &items, Ref<FileMenu> parentMenu, int32 eventID)
	{
		for (int32 i = 0; i < items.size(); ++i)
		{
			Ref<MenuItem> currentItem = items[i];

			if (currentItem->ID == eventID)
			{
				Ref<MenuItem> before = Ref<MenuItem>::Create(currentItem->Name, currentItem->ID, currentItem->Callback, currentItem->Visible, currentItem->Separator);
				before->IsSelected = currentItem->IsSelected;
				before->IsSubmenu = currentItem->IsSubmenu;
				before->SubmenuItems = currentItem->SubmenuItems;
				before->Shortcut = currentItem->Shortcut;
				currentItem->Callback(parentMenu.Get(), currentItem.Get());
				if (!before.Equals(items[i]))
				{
					FileMenuChangedEvent e(currentItem);
					HLApplication::Get().GetWindow().GetEventCallback()(e);
				}

				return true;
			}
		}

		return false;
	}
}

#endif // HL_PLATFORM_WINDOWS