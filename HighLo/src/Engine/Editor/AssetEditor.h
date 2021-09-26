// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Core/Time.h"
#include "Engine/Events/Events.h"

#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"

namespace highlo
{
	class AssetEditor
	{
	public:

		HLAPI virtual ~AssetEditor() {}

		HLAPI virtual void OnUpdate(Timestep ts) {}
		HLAPI virtual void OnUIRender(Timestep ts);
		HLAPI virtual void OnEvent(Event &e) {}

		HLAPI void SetOpen(bool shouldOpen);
		HLAPI virtual void SetAsset(const Ref<Asset> &asset) = 0;

	protected:

		HLAPI AssetEditor(const HLString &title);

		HLAPI void SetMinSize(uint32 width, uint32 height);
		HLAPI void SetMaxSize(uint32 width, uint32 height);

		HLAPI const HLString &GetTitle() const { return m_Title; }
		HLAPI void SetTitle(const HLString &title) { m_Title = title; }

	private:

		virtual void OnClose() = 0;
		virtual void Render() = 0;
		virtual ImGuiWindowFlags GetWindowFlags() { return m_WindowFlags; }

		HLString m_Title;
		bool m_IsOpen = false;

		ImGuiWindowFlags m_WindowFlags = 0;
		ImVec2 m_MinSize, m_MaxSize;
	};
}

