#pragma once

#include "Engine/Renderer/Light.h"

namespace highlo
{
	struct UniformBufferCamera
	{
		glm::mat4 ViewProjection;
		glm::mat4 InverseViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
	};

	struct UniformBufferHBAOData
	{
		glm::vec4 PerspectiveInfo;
		glm::vec2 InvQuarterResolution;
		float RadiusToScreen;
		float NegInvR2;
		float NDotVBias;
		float AOMultiplier;
		float PowExponent;
		bool IsOrtho;
		glm::vec4 Float2Offsets[16];
		glm::vec4 Jitters[16];
		glm::vec3 Padding;
		float ShadowTolerance;
	};

	struct UniformBufferShadow
	{
		glm::mat4 ViewProjection[4];
	};

	struct UniformBufferScene
	{
		DirLight Lights;
		glm::vec3 u_CameraPosition;
		float EnvMapIntensity = 1.0f;
	};

	struct UniformBufferPointLights
	{
		uint32 LightCount = 0;
		glm::vec3 Padding{};
		PointLight PointLights[1024]{};
	};

	struct UniformBufferRendererData
	{
		glm::vec4 CascadeSplits;
		uint32 TilesCountX{ 0 };
		bool ShowCascades = false;
		char Padding0[3] = { 0, 0, 0 };
		bool SoftShadows = true;
		char Padding1[3] = { 0, 0, 0 };
		float LightSize = 0.5f;
		float MaxShadowDistance = 200.0f;
		float ShadowFade = 1.0f;
		bool CascadeFading = true;
		char Padding2[3] = { 0, 0, 0 };
		float CascadeTransitionFade = 1.0f;
		bool ShowLightComplexity = false;
		char Padding3[3] = { 0, 0, 0 };
	};

	struct UniformBufferScreenData
	{
		glm::vec2 InvFullResolution;
		glm::vec2 FullResolution;
		glm::vec2 InvHalfResolution;
		glm::vec2 HalfResolution;
	};
}

