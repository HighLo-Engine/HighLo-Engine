#pragma once

#include "MenuItem.h"
#include "Engine/Core/HLCore.h"

namespace highlo
{
	class FileMenu : public IsSharedReference
	{
	public:

		HLAPI FileMenu() = default;
		HLAPI virtual ~FileMenu() = default;

		HLAPI virtual void AddMenuItem(const Ref<MenuItem> &item) = 0;
		HLAPI virtual void AddMenuItem(const MenuItem &item) = 0;
		HLAPI virtual void AddMenuItem(const HLString &name, int32 id, MenuItemCallback callback, bool visible = true) = 0;
		
		HLAPI virtual void AddSubMenu(const Ref<FileMenu> &other) = 0;
		HLAPI virtual void AddMenuSeperator() = 0;

		HLAPI virtual void *GetMenuHandle() = 0;
		HLAPI virtual void *GetMenuHandle() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI virtual const std::vector<MenuItem> &GetMenuItems() const = 0;

		HLAPI virtual bool EnableMenuItem(int32 id, bool bEnabled) = 0;
		HLAPI static Ref<FileMenu> Create(const HLString &name);
	};
}