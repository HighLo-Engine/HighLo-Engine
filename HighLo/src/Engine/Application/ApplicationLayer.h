// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Events/Events.h"
#include "Engine/Core/Time.h"

namespace highlo
{
	class ApplicationLayer
	{
	public:

		HLAPI ApplicationLayer(const HLString &debugName);
		HLAPI virtual ~ApplicationLayer() = default;

		HLAPI virtual void OnAttach() {}
		HLAPI virtual void OnDetach() {}
		HLAPI virtual void OnUpdate(Timestep ts) {}
		HLAPI virtual void OnUIRender(Timestep ts) {}
		HLAPI virtual void OnEvent(Event &e) {}
		HLAPI virtual void OnResize(uint32 width, uint32 height) {}

	protected:

		HLString m_DebugName;
	};
}

