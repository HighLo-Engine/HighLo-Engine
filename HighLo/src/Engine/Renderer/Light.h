// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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
		glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		float ShadowAmount = 1.0f;
		// C++ only
		bool CastShadows = true;
	};

	struct DirLight
	{
		glm::vec3 Direction;
		float Padding = 0.0f;
		glm::vec3 Radiance;
		float Multiplier;
	};

	struct PointLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float MinRadius = 0.001f;
		float Radius = 25.0f;
		float Falloff = 1.f;
		float SourceSize = 0.1f;
		bool CastsShadows = true;
		char Padding[3]{ 0, 0, 0 };
	};

	struct SpotLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
		float AngleAttenuation = 0.0f;
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float Range = 0.1f;
		float Angle = 0.0f;
		float Falloff = 1.0f;
		bool SoftShadows = true;
		char Padding0[3]{ 0, 0, 0 };
		bool CastsShadows = true;
		char Padding1[3]{ 0, 0, 0 };
	};

	struct LightEnvironment
	{
		static constexpr size_t MaxDirectionalLights = 4;

		DirectionalLight DirectionalLights[MaxDirectionalLights];
		std::vector<PointLight> PointLights;
		std::vector<SpotLight> SpotLights;

		[[nodiscard]] uint32 GetPointLightsSize() const { return (uint32)(PointLights.size() * sizeof(PointLight)); }
		[[nodiscard]] uint32 GetSpotLightsSize() const { return (uint32)(SpotLights.size() * sizeof(SpotLight)); }
	};
}
