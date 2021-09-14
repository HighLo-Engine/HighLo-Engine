// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct MenuItem;
	class FileMenu;

	using MenuItemCallback = std::function<void(FileMenu*, MenuItem*)>;

	struct MenuItem
	{
		HLString Name = "Default Menu Item";
		HLString Shortcut = "";
		
		int32 ID = 0;
		bool Visible = true;
		bool Separator = false;
		bool IsSubmenu = false;

		MenuItemCallback Callback = nullptr;

		std::vector<MenuItem> SubmenuItems;

		HLAPI MenuItem() = default;
		HLAPI MenuItem(const MenuItem&) = default;

		HLAPI MenuItem(const HLString &name, int32 id, MenuItemCallback callback, bool visible = true, bool separator = false)
			: Name(name), ID(id), Visible(visible), Separator(separator), Callback(callback) {}

	};
}