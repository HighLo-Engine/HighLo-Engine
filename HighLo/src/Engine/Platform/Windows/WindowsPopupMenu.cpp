#include "HighLoPch.h"
#include "WindowsPopupMenu.h"

#ifdef HL_PLATFORM_WINDOWS

#include "Engine/Core/HLInput.h"

namespace highlo
{
	WindowsPopupMenu::WindowsPopupMenu(const HLString &name)
		: m_Name(name)
	{
		m_NativeHandle = CreatePopupMenu();
	}
	
	WindowsPopupMenu::~WindowsPopupMenu() {}

	void WindowsPopupMenu::AddMenuItem(const Ref<PopupMenuItem> &item)
	{
		m_LastPosition += item->Position;
		if (item->Visible)
			InsertMenuW(m_NativeHandle, item->Position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, item->ID, item->Name.W_Str());
		else
			InsertMenuW(m_NativeHandle, item->Position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, item->ID, item->Name.W_Str());
	}

	void WindowsPopupMenu::AddMenuItem(const PopupMenuItem &item)
	{
		m_LastPosition += item.Position;
		if (item.Visible)
			InsertMenuW(m_NativeHandle, item.Position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, item.ID, item.Name.W_Str());
		else
			InsertMenuW(m_NativeHandle, item.Position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, item.ID, item.Name.W_Str());
	}

	void WindowsPopupMenu::AddMenuItem(const HLString &name, int32 id, int32 position, bool visible)
	{
		m_LastPosition += position;
		if (visible)
			InsertMenuW(m_NativeHandle, position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS, id, name.W_Str());
		else
			InsertMenuW(m_NativeHandle, position, MF_BYPOSITION | MF_STRING | MNS_NOTIFYBYPOS | MF_GRAYED, id, name.W_Str());
	}
	
	void WindowsPopupMenu::AddSeperator()
	{
		InsertMenuW(m_NativeHandle, m_LastPosition + 1, MF_SEPARATOR, 0, NULL);
	}
	
	void WindowsPopupMenu::AddSubMenu(const Ref<PopupMenu> &menu)
	{
		HMENU item = (HMENU)menu->GetPopupMenuHandle();
		InsertMenuW(m_NativeHandle, m_LastPosition + 1, MF_STRING | MF_POPUP | MNS_NOTIFYBYPOS, (UINT_PTR) item, menu->GetName().W_Str());
	}
	
	void WindowsPopupMenu::Show()
	{
		if (Input::IsMouseButtonPressed(HLMouseButtonCode::BUTTON_RIGHT))
		{
			HWND window = GetActiveWindow();
			SetForegroundWindow(window);

			POINT p;
			GetCursorPos(&p);

			TrackPopupMenuEx(m_NativeHandle, TPM_RIGHTBUTTON | TPM_RECURSE, p.x, p.y, window, NULL);
		}
	}
}

#endif  // HL_PLATFORM_WINDOWS