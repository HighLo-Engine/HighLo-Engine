// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-03) initial release
//

#pragma once

#include "ImGui.h"

namespace highlo::UI
{
	HLAPI bool TreeNodeWithIcon(const Ref<Texture2D> &icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char *label, const char *labelEnd, ImColor iconTint = IM_COL32_WHITE);
	HLAPI bool TreeNodeWithIcon(const Ref<Texture2D> &icon, const void *ptrID, ImGuiTreeNodeFlags flags, ImColor iconTint, const char *format, ...);
	HLAPI bool TreeNodeWithIcon(const Ref<Texture2D> &icon, const char *label, ImGuiTreeNodeFlags flags, ImColor iconTint = IM_COL32_WHITE);
}

