#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	using MenuItemCallback = std::function<void()>;

	struct MenuItem
	{
		HLString Name = "Default Menu Item";
		HLString Shortcut = "";
		
		int32 ID = 0;
		bool Visible = true;
		bool Seperator = false;

		MenuItemCallback Callback = nullptr;

		HLAPI MenuItem() = default;
		HLAPI MenuItem(const MenuItem&) = default;

		HLAPI MenuItem(const HLString &name, int32 id, MenuItemCallback callback, bool visible = true, bool seperator = false)
			: Name(name), ID(id), Visible(visible), Seperator(seperator), Callback(callback) {}

	};
}