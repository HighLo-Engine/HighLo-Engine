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

		if (item->Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item->ID, item->Name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item->ID, item->Name.W_Str());

		m_MenuItems.push_back(*item);
	}

	void WindowsFileMenu::AddMenuItem(const MenuItem &item)
	{
		if (!m_NativeHandle)
			return;

		if (item.Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item.ID, item.Name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item.ID, item.Name.W_Str());

		m_MenuItems.push_back(item);
	}
	
	void WindowsFileMenu::AddMenuItem(const HLString &name, int32 id, MenuItemCallback callback, bool visible)
	{
		if (!m_NativeHandle)
			return;

		MenuItem item;
		item.Name = name;
		item.ID = id;
		item.Visible = visible;
		item.Seperator = false;
		item.Callback = callback;
		m_MenuItems.push_back(item);

		if (item.Visible)
			AppendMenuW(m_NativeHandle, MF_STRING, item.ID, item.Name.W_Str());
		else
			AppendMenuW(m_NativeHandle, MF_STRING | MF_GRAYED, item.ID, item.Name.W_Str());
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