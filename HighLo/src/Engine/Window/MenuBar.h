#pragma once

#include "Engine/Core/HLCore.h"
#include "FileMenu.h"

namespace highlo
{
	class MenuBar : public IsSharedReference
	{
	public:

		virtual ~MenuBar() = default;

		virtual void AddMenu(const Ref<FileMenu> &menu) = 0;
		virtual void RemoveMenu(const Ref<FileMenu> &menu) = 0;

		virtual void *GetNativeMenuBar() = 0;
		virtual void *GetNativeMenuBar() const = 0;

		static Ref<MenuBar> Create();
	};
}