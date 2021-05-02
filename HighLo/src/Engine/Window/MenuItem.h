#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	struct MenuItem
	{
		HLString Name = "Default Menu Item";
		
		int32 ID = 0;
		bool Visible = true;
		bool Seperator = false;

		MenuItem() = default;
		MenuItem(const MenuItem&) = default;

		MenuItem(const HLString &name, int32 id, bool visible = true, bool seperator = false)
			: Name(name), ID(id), Visible(visible), Seperator(seperator) {}

	};
}