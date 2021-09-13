#pragma once

#include "Engine/Renderer/Model.h"
#include "Engine/Camera/Camera.h"

namespace highlo
{
	struct IDComponent
	{
		uint64 ID = 0;
	};

	struct SceneComponent
	{
		uint64 SceneID = 0;
	};

	struct TransformComponent
	{
		Transform Transform;
	};

	struct TagComponent
	{
		HLString Tag;
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
