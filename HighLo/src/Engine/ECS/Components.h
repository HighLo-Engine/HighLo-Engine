#pragma once
#include "Engine/Renderer//Model.h"

namespace highlo
{
	struct TransformComponent
	{
		Transform Transform;
	};

	struct RenderComponent
	{
		Model Model;
	};
}
