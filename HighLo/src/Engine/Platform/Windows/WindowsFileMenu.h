#pragma once

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include "Engine/Window/FileMenu.h"

namespace highlo
{
	class WindowsFileMenu : public FileMenu
	{
	public:

		WindowsFileMenu() = default;
		WindowsFileMenu(const HLString &name);
		virtual ~WindowsFileMenu();

		virtual void AddMenuItem(const Ref<MenuItem> &item) override;
		virtual void AddMenuItem(const MenuItem &item) override;
		virtual void AddMenuItem(const HLString &name, int32 id, bool visible = true) override;

		virtual void AddSubMenu(const Ref<FileMenu> &other) override;
		virtual void AddMenuSeperator() override;

		virtual bool EnableMenuItem(int32 id, bool bEnabled) override;

		virtual void *GetMenuHandle() override { return (void*)m_NativeHandle; }
		virtual void *GetMenuHandle() const override { return (void*)m_NativeHandle; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }
		
	private:

		HLString m_Name = "DefaultMenuTitle";
		HMENU m_NativeHandle;

	};
}

#endif // HL_PLATFORM_WINDOWS