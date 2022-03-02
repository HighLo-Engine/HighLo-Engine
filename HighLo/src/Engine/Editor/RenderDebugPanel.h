#pragma once

#include "Engine/Events/Events.h"

namespace highlo
{
	class RenderDebugPanel
	{
	public:

		HLAPI RenderDebugPanel();
		HLAPI virtual ~RenderDebugPanel();

		HLAPI void OnUIRender(bool *show);
		HLAPI void OnEvent(Event &e);
	};
}

