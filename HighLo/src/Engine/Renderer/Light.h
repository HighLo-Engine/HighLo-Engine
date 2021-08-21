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

	struct LightEnvironment
	{
		DirectionalLight DirectionalLight[4];
	};
}
