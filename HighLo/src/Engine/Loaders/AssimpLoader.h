#pragma once
#include "Engine/Renderer/Model.h"

namespace highlo
{
	class AssimpLoader
	{
	public:
		static Model LoadStatic(const HLString& filepath, bool bShouldApplyCorrectionMatrix = false);
	};
}
