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
		Ref<Model> MainModel;
		Ref<Model> ModelLOD1;
		Ref<Model> ModelLOD2;
		bool UseLOD = false;
		int32 LodLevel = 0;
		bool Enabled = true;
		bool Wireframe = false;
		bool ResetForNextFrame = false;
	};

	struct SceneComponent
	{
		uint64 SceneID;
	};


}
