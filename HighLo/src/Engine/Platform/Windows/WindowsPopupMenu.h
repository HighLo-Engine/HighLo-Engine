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
		virtual void AddMenuItem(const HLString &name, int32 id, int32 position, bool visible = true) override;
		virtual void AddSeperator() override;
		virtual void AddSubMenu(const Ref<PopupMenu> &menu) override;

		virtual void Show() override;

		virtual void *GetPopupMenuHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetPopupMenuHandle() const override { return (void*)m_NativeHandle; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		HMENU m_NativeHandle;
		HLString m_Name;
		int32 m_LastPosition = 0;
	};
}

#endif // HL_PLATFORM_WINDOWS