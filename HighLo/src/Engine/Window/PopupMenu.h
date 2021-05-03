#pragma once

#include "PopupMenuItem.h"

namespace highlo
{
	class PopupMenu : public IsSharedReference
	{
	public:

		virtual ~PopupMenu() = default;

		HLAPI virtual void AddMenuItem(const Ref<PopupMenuItem> &item) = 0;
		HLAPI virtual void AddMenuItem(const PopupMenuItem &item) = 0;
		HLAPI virtual void AddMenuItem(const HLString &name, int32 id, int32 position, bool visible = true) = 0;
		HLAPI virtual void AddSeperator() = 0;
		HLAPI virtual void AddSubMenu(const Ref<PopupMenu> &menu) = 0;

		HLAPI virtual void Show() = 0;

		HLAPI virtual void *GetPopupMenuHandle() = 0;
		HLAPI virtual void *GetPopupMenuHandle() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI static Ref<PopupMenu> Create(const HLString &name);
	};
}