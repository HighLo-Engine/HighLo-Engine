#pragma once

#include "PopupMenuItem.h"

namespace highlo
{
	class PopupMenu : public IsSharedReference
	{
	public:

		virtual ~PopupMenu() = default;

		virtual void AddMenuItem(const Ref<PopupMenuItem> &item) = 0;
		virtual void AddMenuItem(const PopupMenuItem &item) = 0;
		virtual void AddMenuItem(const HLString &name, int32 id, int32 position, bool visible = true) = 0;
		virtual void AddSeperator() = 0;
		virtual void AddSubMenu(const Ref<PopupMenu> &menu) = 0;

		virtual void Show() = 0;

		virtual void *GetPopupMenuHandle() = 0;
		virtual void *GetPopupMenuHandle() const = 0;

		virtual HLString &GetName() = 0;
		virtual const HLString &GetName() const = 0;

		static Ref<PopupMenu> Create(const HLString &name);
	};
}