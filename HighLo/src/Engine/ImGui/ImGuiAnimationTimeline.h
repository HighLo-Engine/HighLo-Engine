// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-02) initial release
//

#pragma once

#include "ImGui.h"

enum ImGuiAnimationTimelineFlags_
{
	ImGuiAnimationTimelineFlags_None = 0,
	ImGuiAnimationTimelineFlags_Test = HL_BIT(0),
};

typedef int32 ImGuiAnimationTimelineFlags;

namespace highlo::UI
{
	bool BeginAnimationTimeline(const char *id, ImVec2 size, ImGuiAnimationTimelineFlags flags = 0);
	void EndAnimationTimeline();
}

