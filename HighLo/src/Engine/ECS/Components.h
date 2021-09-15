// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-15) Changed ID Types to UUID, Removed TagComponent
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Model.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Core/UUID.h"

namespace highlo
{
	struct IDComponent
	{
		UUID ID = 0;
	};

	struct SceneComponent
	{
		UUID SceneID = 0;
	};

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

	struct CameraComponent
	{
		Ref<Camera> Camera;
		bool Primary = true;
	};

	struct SpriteComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		Ref<Texture2D> Texture;
	};

	struct ModelComponent
	{
		Ref<Model> Model;
		bool IsFractured = false;
	};

	struct RelationshipComponent
	{
		UUID ParentHandle = 0;
		std::vector<UUID> Children;
	};


}
