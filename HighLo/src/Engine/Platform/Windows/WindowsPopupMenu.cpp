#include "HighLoPch.h"
#include "WindowsPopupMenu.h"

#ifdef HL_PLATFORM_WINDOWS

#include "Engine/Core/HLInput.h"

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
	
	void WindowsPopupMenu::Update()
	{
		if (Input::IsMouseButtonPressed(HL_MOUSE_BUTTON_RIGHT))
		{
			HWND window = GetActiveWindow();
			SetForegroundWindow(window);

			POINT p;
			GetCursorPos(&p);

			TrackPopupMenuEx(m_NativeHandle, TPM_LEFTBUTTON | TPM_RECURSE, p.x, p.y, window, NULL);
		}
	}

	void WindowsPopupMenu::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<FileMenuEvent>(HL_BIND_EVENT_FUNCTION(WindowsPopupMenu::OnFileMenuClickedEvent));
	}

	bool WindowsPopupMenu::OnFileMenuClickedEvent(const FileMenuEvent &e)
	{
		for (PopupMenuItem item : m_MenuItems)
		{
			if (item.ID == e.GetID())
			{
				item.Callback();
				return true;
			}
		}

		return false;
	}
}

#endif  // HL_PLATFORM_WINDOWS