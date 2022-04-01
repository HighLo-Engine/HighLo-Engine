// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "PopupMenuItem.h"
#include "Engine/Events/ApplicationEvents.h"

namespace highlo
{
	class PopupMenu : public IsSharedReference
	{
	public:

		virtual ~PopupMenu() = default;

		HLAPI virtual void AddMenuItem(const Ref<PopupMenuItem> &item) = 0;
		HLAPI virtual void AddMenuItem(const PopupMenuItem &item) = 0;
		HLAPI virtual void AddMenuItem(const HLString &name, int32 id, PopupItemCallback callback, bool visible = true) = 0;
		HLAPI virtual void AddSeparator() = 0;
		HLAPI virtual void AddSubMenu(const Ref<PopupMenu> &menu) = 0;

		HLAPI virtual void Update() const = 0;
		HLAPI virtual void OnEvent(Event &e) = 0;

		HLAPI virtual std::vector<PopupMenuItem> &GetMenuItems() = 0;
		HLAPI virtual const std::vector<PopupMenuItem> &GetMenuItems() const = 0;

		HLAPI virtual void *GetPopupMenuHandle() = 0;
		HLAPI virtual void *GetPopupMenuHandle() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI static Ref<PopupMenu> Create(const HLString &name);
	};
}
