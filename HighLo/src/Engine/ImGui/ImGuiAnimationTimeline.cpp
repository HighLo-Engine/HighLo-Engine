// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "ImGuiAnimationTimeline.h"

namespace highlo::UI
{
	bool BeginAnimationTimeline(const char *id, ImVec2 size, ImGuiAnimationTimelineFlags flags)
	{
		ImGui::PushID(id);

	//	ImGui::SetNextWindowSize(size);

		ImGui::PopID();
		return false;
	}

	void EndAnimationTimeline()
	{
	}
}

