// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-19) Added Prefab Component
//     - 1.1 (2021-09-15) Changed ID Types to UUID, Removed TagComponent
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Meshes/DynamicModel.h"
#include "Engine/Renderer/Meshes/StaticModel.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Core/UUID.h"

namespace highlo
{
	struct PrefabComponent
	{
		UUID PrefabID = 0;
		UUID EntityID = 0;
	};

	struct SceneComponent
	{
		UUID SceneID = 0;
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = true;
	};

	struct SpriteComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float TilingFactor = 1.0f;
		Ref<Texture2D> Texture;
	};

	struct StaticModelComponent
	{
		UUID Model; // contains the asset ID, which is maintained by the AssetManager
		Ref<MaterialTable> Materials = MaterialTable::Create();
	};

	struct DynamicModelComponent
	{
		UUID Model; // contains the asset ID, which is maintained by the AssetManager
		uint32 SubmeshIndex = 0;
		Ref<MaterialTable> Materials = MaterialTable::Create();
		bool IsFractured = false;
	};

	struct DirectionalLightComponent
	{
		glm::vec3 Radiance = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		bool CastShadows = true;
		bool SoftShadows = true;
		float LightSize = 0.5f;
	};

	struct PointLightComponent
	{
		glm::vec3 Radiance = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
		float Radius = 10.0f;
		float LightSize = 0.5f;
		float MinRadius = 1.0f;
		float MaxRadius = 10.0f;
		bool CastShadows = true;
		bool SoftShadows = true;
		float Falloff = 1.0f;
	};

	struct SkyLightComponent
	{
		AssetHandle SceneEnvironment;
		float Intensity = 1.0f;
		float Lod = 0.0f;

		bool DynamicSky = false;
		glm::vec3 TurbityAzimuthInclination = { 2.0f, 0.0f, 0.0f };
	};

	struct TextComponent
	{
		HLString Text = "Text";
		AssetHandle FontAsset;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float LineSpacing = 0.0f;
		float Kerning = 0.0f;
		float MaxWidth = 10.0f;
	};


}
