#include "HighLoPch.h"
#include "WindowsMenuBar.h"

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
		// TODO: Remove from m_Menus
	}

	void WindowsMenuBar::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<FileMenuEvent>(HL_BIND_EVENT_FUNCTION(WindowsMenuBar::OnFileMenuClicked));
	}

	bool WindowsMenuBar::OnFileMenuClicked(FileMenuEvent &e)
	{
		int32 eventID = e.GetID();
		for (int32 i = 0; i < m_Menus.size(); ++i)
		{
			std::vector<MenuItem> menuItems = m_Menus[i]->GetMenuItems();
			for (int32 j = 0; j < menuItems.size(); ++j)
			{
				if (eventID == menuItems[j].ID)
				{
					menuItems[j].Callback();
					return true;
				}
			}
		}

		return false;
	}
}

#endif // HL_PLATFORM_WINDOWS