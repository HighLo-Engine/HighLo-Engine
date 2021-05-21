#pragma once
#include "Engine/Renderer/Model.h"

namespace highlo
{
	class AssimpLoader
	{
	public:
		HLAPI static Ref<Model> LoadStatic(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
		HLAPI static Ref<Mesh> LoadAnimated(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}
