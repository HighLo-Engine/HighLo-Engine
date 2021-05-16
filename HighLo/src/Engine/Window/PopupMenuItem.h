#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	using PopupItemCallback = std::function<void()>;

	struct PopupMenuItem
	{
		HLString Name = "DefaultPopupMenuItem";
		int32 ID = 0;
		bool Visible = true;
		bool Seperator = false;
		PopupItemCallback Callback = nullptr;

		HLAPI PopupMenuItem() = default;
		HLAPI PopupMenuItem(const PopupMenuItem&) = default;

		HLAPI PopupMenuItem(const HLString &name, int32 id, PopupItemCallback callback, bool visible = true, bool seperator = false)
			: Name(name), ID(id), Callback(callback), Visible(visible), Seperator(seperator) {}
	};
}