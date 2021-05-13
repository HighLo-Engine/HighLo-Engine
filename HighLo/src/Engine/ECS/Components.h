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
		Model MainModel;
		Model ModelLOD1;
		Model ModelLOD2;
		bool UseLOD = false;
		int LodLevel = 0;
		bool Enabled = true;
		bool Wireframe = false;
		bool ResetForNextFrame = false;
	};
}
