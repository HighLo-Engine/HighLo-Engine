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
	}

	void WindowsMenuBar::RemoveMenu(const Ref<FileMenu> &menu)
	{
		DestroyMenu((HMENU)menu->GetMenuHandle());
	}
}

#endif // HL_PLATFORM_WINDOWS