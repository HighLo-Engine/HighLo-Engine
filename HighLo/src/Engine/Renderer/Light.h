// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-15) Added PointLight
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Math/Math.h"

namespace highlo
{
	struct Light
	{
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		glm::vec3 Position = { 0.0f, 10.0f, -10.0f };
		float Intensity = 1000.0f;
	};

	struct DirectionalLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float Intensity = 1.0f;

		bool CastShadows = true;
	};

	struct PointLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Multiplier = 1.0f;
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float MinRadius = 0.001f;
		float Radius = 25.0f;
		float Falloff = 1.0f;
		float SourceSize = 0.1f;
		bool CastShadows = true;
		char Padding[3]{ 0, 0, 0 };
	};

	struct LightEnvironment
	{
		DirectionalLight DirectionalLights[4];
		std::vector<PointLight> PointLights;
	};


}
