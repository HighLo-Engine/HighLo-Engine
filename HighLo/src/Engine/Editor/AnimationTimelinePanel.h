// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-01) initial release
//

#pragma once

#include "Engine/Events/Events.h"

namespace highlo
{
	struct AnimationTimelineSettings
	{

	};

	class AnimationTimelinePanel
	{
	public:

		HLAPI AnimationTimelinePanel();
		HLAPI virtual ~AnimationTimelinePanel();

		HLAPI void OnUIRender(bool *show);
		HLAPI void OnEvent(Event &e);

	private:

		bool m_Dragging = false;
		bool m_Scrolled = false;
		bool m_AnimPlaying = false;
		bool m_AnimPausing = false;
		
		float m_Zoom = 50.0f;
		float m_Offset = 0.0f;
		float m_ButtonPos = 0.0f;
		float m_RectSize = 700.0f;

		float m_Progress = 0.0f;
		int32 m_NumFrames = 1;
		int32 m_BeginFrame = 0;
		int32 m_EndFrame = 1000;
	};
}

