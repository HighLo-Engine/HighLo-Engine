#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	struct PopupMenuItem
	{
		HLString Name = "DefaultPopupMenuItem";
		int32 ID = 0;
		int32 Position = 0;
		bool Visible = true;
		bool Seperator = false;

		PopupMenuItem() = default;
		PopupMenuItem(const PopupMenuItem&) = default;

		PopupMenuItem(const HLString &name, int32 id, int32 position, bool visible = true, bool seperator = false)
			: Name(name), ID(id), Position(position), Visible(visible), Seperator(seperator) {}
	};
}