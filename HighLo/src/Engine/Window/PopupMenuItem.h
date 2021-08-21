#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct PopupMenuItem;
	using PopupItemCallback = std::function<void(PopupMenuItem)>;

	struct PopupMenuItem
	{
		HLString Name = "DefaultPopupMenuItem";
		int32 ID = 0;
		bool Visible = true;
		bool Separator = false;
		bool IsSubmenu = false;
		PopupItemCallback Callback = nullptr;

		std::vector<PopupMenuItem> SubMenuItems;

		HLAPI PopupMenuItem() = default;
		HLAPI PopupMenuItem(const PopupMenuItem&) = default;

		HLAPI PopupMenuItem(const HLString &name, int32 id, PopupItemCallback callback, bool visible = true, bool separator = false)
			: Name(name), ID(id), Callback(callback), Visible(visible), Separator(separator) {}
	};
}