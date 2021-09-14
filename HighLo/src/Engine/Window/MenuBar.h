// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Events.h"
#include "FileMenu.h"

namespace highlo
{
	class MenuBar : public IsSharedReference
	{
	public:

		HLAPI virtual ~MenuBar() = default;

		HLAPI virtual void AddMenu(const Ref<FileMenu> &menu) = 0;
		HLAPI virtual void RemoveMenu(const Ref<FileMenu> &menu) = 0;

		HLAPI virtual std::vector<Ref<FileMenu>> &GetMenus() = 0;
		HLAPI virtual const std::vector<Ref<FileMenu>> &GetMenus() const = 0;

		HLAPI virtual void *GetNativeMenuBar() = 0;
		HLAPI virtual void *GetNativeMenuBar() const = 0;

		HLAPI virtual void OnEvent(Event &e) = 0;

		HLAPI static Ref<MenuBar> Create();
	};
}