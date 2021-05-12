#pragma once
#include "Engine/Renderer/Model.h"

namespace highlo
{
	struct TransformComponent
	{
		Transform Transform;
	};

	struct RenderComponent
	{
		Model Model;
		bool Enabled = true;
		bool Wireframe = false;
	};
}
