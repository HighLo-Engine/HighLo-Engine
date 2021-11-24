// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Engine/Window/Window.h"

namespace highlo
{
	class ImGuiRenderer : public IsSharedReference
	{
	public:

		HLAPI virtual ~ImGuiRenderer() = default;

		HLAPI virtual void Init(Window *window) = 0;
		HLAPI virtual void Shutdown() = 0;

		HLAPI virtual void NewFrame() = 0;

		HLAPI virtual void Render() = 0;
		HLAPI virtual void RenderDrawData() = 0;

		static Ref<ImGuiRenderer> Create();
	};
}

