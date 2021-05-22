#include "HighLoPch.h"
#include "WindowsFileMenu.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	WindowsFileMenu::WindowsFileMenu(const HLString &name)
		: m_Name(name)
	{
		m_NativeHandle = CreatePopupMenu();
	}
	
	WindowsFileMenu::~WindowsFileMenu()
	{
		DestroyMenu(m_NativeHandle);
	}

	void WindowsFileMenu::AddMenuItem(const Ref<MenuItem> &item)
	{
		if (!m_NativeHandle)
			return;

		HLString name = item->Name + "\t" + item->Shortcut;

		if (item->Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item->ID, name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item->ID, name.W_Str());

		m_MenuItems.push_back(*item);
	}

	void WindowsFileMenu::AddMenuItem(const MenuItem &item)
	{
		if (!m_NativeHandle)
			return;

		HLString name = item.Name + "\t" + item.Shortcut;

		if (item.Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item.ID, name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item.ID, name.W_Str());

		m_MenuItems.push_back(item);
	}
	
	void WindowsFileMenu::AddMenuItem(const HLString &name, const HLString &shortcut, int32 id, MenuItemCallback callback, bool visible)
	{
		if (!m_NativeHandle)
			return;

		MenuItem item;
		item.Name = name;
		item.Shortcut = shortcut;
		item.ID = id;
		item.Visible = visible;
		item.Seperator = false;
		item.Callback = callback;
		m_MenuItems.push_back(item);

		HLString realName = name + "\t" + shortcut;

		if (item.Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item.ID, realName.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item.ID, realName.W_Str());
	}
	
	void WindowsFileMenu::AddSubMenu(const Ref<FileMenu> &other)
	{
		HMENU menuItem = (HMENU) other->GetMenuHandle();
		AppendMenuW(m_NativeHandle, MF_POPUP, (UINT_PTR) menuItem, other->GetName().W_Str());

		std::vector<MenuItem> otherMenuItems = other->GetMenuItems();
		for (int32 i = 0; i < otherMenuItems.size(); ++i)
		{
			m_MenuItems.push_back(otherMenuItems[i]);
		}
	}
	
	void WindowsFileMenu::AddMenuSeperator()
	{
		AppendMenuW(m_NativeHandle, MF_SEPARATOR, 0, NULL);

		MenuItem item;
		item.Name = "";
		item.Shortcut = "";
		item.ID = -1;
		item.Visible = true;
		item.Seperator = true;
		item.Callback = nullptr;
		m_MenuItems.push_back(item);
	}

	bool WindowsFileMenu::EnableMenuItem(int32 id, bool bEnabled)
	{
		if (bEnabled)
			return ::EnableMenuItem(m_NativeHandle, id, MF_ENABLED);
		else
			return ::EnableMenuItem(m_NativeHandle, id, MF_GRAYED);
	}
}

#endif // HL_PLATFORM_WINDOWS