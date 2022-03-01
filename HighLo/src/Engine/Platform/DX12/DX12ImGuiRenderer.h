// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/ImGui/ImGuiRenderer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12ImGuiRenderer : public ImGuiRenderer
	{
	public:

		DX12ImGuiRenderer();
		virtual ~DX12ImGuiRenderer();

		virtual void Init(Window *window) override;
		virtual void Shutdown() override;

		virtual void NewFrame() override;

		virtual void Render() override;
		virtual void RenderDrawData() override;
	};
}

#endif // HIGHLO_API_DX12

