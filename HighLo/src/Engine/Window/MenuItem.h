// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-08) Added equal operators and copy operator
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct MenuItem;
	class FileMenu;

	using MenuItemCallback = std::function<void(FileMenu*, MenuItem*)>;

	struct MenuItem : public IsSharedReference
	{
		HLString Name = "Default Menu Item";
		HLString Shortcut = "";
		
		int32 ID = 0;
		bool Visible = true;
		bool Separator = false;
		bool IsSubmenu = false;
		bool IsSelected = false;

		MenuItemCallback Callback = nullptr;

		std::vector<Ref<MenuItem>> SubmenuItems;

		HLAPI MenuItem() = default;
		HLAPI MenuItem(const MenuItem&) = default;
		HLAPI MenuItem &operator=(const MenuItem&) = default;

		HLAPI MenuItem(const HLString &name, int32 id, const MenuItemCallback &callback, bool visible = true, bool separator = false)
			: Name(name), ID(id), Visible(visible), Separator(separator), Callback(callback) {}

		HLAPI bool operator==(const MenuItem &other) const
		{
			return Name == other.Name
				&& Shortcut == other.Shortcut
				&& Visible == other.Visible
				&& Separator == other.Separator
				&& IsSubmenu == other.IsSubmenu
				&& IsSelected == other.IsSelected
				&& SubmenuItems == other.SubmenuItems;
		}

		HLAPI bool operator!=(const MenuItem &other) const
		{
			return !(*this == other);
		}
	};
}

