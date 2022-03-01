// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

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
