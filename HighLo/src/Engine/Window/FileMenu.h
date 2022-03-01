// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "MenuItem.h"
#include "Engine/Core/Core.h"

namespace highlo
{
	class FileMenu : public IsSharedReference
	{
	public:

		HLAPI FileMenu() = default;
		HLAPI virtual ~FileMenu() = default;

		HLAPI virtual void AddMenuItem(const Ref<MenuItem> &item) = 0;
		HLAPI virtual void AddMenuItem(const HLString &name, const HLString &shortcut, int32 id, const MenuItemCallback &callback, bool visible = true) = 0;
		
		HLAPI virtual void AddSubMenu(const Ref<FileMenu> &other) = 0;
		HLAPI virtual void AddSeparator() = 0;

		HLAPI virtual void *GetMenuHandle() = 0;
		HLAPI virtual void *GetMenuHandle() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI virtual std::vector<Ref<MenuItem>> &GetMenuItems() = 0;
		HLAPI virtual Ref<MenuItem> GetMenuItemWithID(int32 id) = 0;
		HLAPI virtual const std::vector<Ref<MenuItem>> &GetMenuItems() const = 0;

		HLAPI virtual bool EnableMenuItem(int32 id, bool bEnabled) = 0;
		HLAPI virtual bool SetCheckmark(int32 id, bool bEnabled) = 0;

		HLAPI static Ref<FileMenu> Create(const HLString &name);
	};
}
