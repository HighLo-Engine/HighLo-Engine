#pragma once

#include "Engine/Core/HLCore.h"
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

		HLAPI virtual void *GetNativeMenuBar() = 0;
		HLAPI virtual void *GetNativeMenuBar() const = 0;

		HLAPI virtual void OnEvent(Event &e) = 0;

		HLAPI static Ref<MenuBar> Create();
	};
}