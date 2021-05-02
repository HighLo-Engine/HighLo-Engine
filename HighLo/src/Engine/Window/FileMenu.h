#pragma once

#include "MenuItem.h"
#include "Engine/Core/HLCore.h"

namespace highlo
{
	class FileMenu
	{
	public:

		FileMenu() = default;
		virtual ~FileMenu() = default;

		virtual void AddMenuItem(const Ref<MenuItem> &item) = 0;
		virtual void AddMenuItem(const MenuItem &item) = 0;
		virtual void AddMenuItem(const HLString &name, int32 id, bool visible = true) = 0;
		
		virtual void AddSubMenu(const Ref<FileMenu> &other) = 0;
		virtual void AddMenuSeperator() = 0;

		virtual void *GetMenuHandle() = 0;
		virtual void *GetMenuHandle() const = 0;

		virtual HLString &GetName() = 0;
		virtual const HLString &GetName() const = 0;

		virtual bool EnableMenuItem(int32 id, bool bEnabled) = 0;
		static Ref<FileMenu> Create(const HLString &name);
	};
}