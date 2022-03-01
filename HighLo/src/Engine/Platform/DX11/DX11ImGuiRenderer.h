#pragma once

#include "Engine/ImGui/ImGuiRenderer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11ImGuiRenderer : public ImGuiRenderer
	{
	public:

		DX11ImGuiRenderer();
		virtual ~DX11ImGuiRenderer();

		virtual void Init(Window *window) override;
		virtual void Shutdown() override;

		virtual void NewFrame() override;

		virtual void Render() override;
		virtual void RenderDrawData() override;
	};
}

#endif // HIGHLO_API_DX11

