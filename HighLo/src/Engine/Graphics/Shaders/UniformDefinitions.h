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
		float LightSize;
		float MaxShadowDistance;
		float ShadowFade;
		float CascadeTransitionFade;
		int32 TilesCountX;
		int32 ShowLightComplexity;
		int32 ShowCascades;
		int32 SoftShadows;
		int32 CascadeFading;
	};

	struct UniformBufferScreenData
	{
		glm::vec2 InvFullResolution;
		glm::vec2 FullResolution;
		glm::vec2 InvHalfResolution;
		glm::vec2 HalfResolution;
	};

	struct UniformBufferSceneCompositeData
	{
		float Exposure;
		float BloomIntensity;
		float BloomDirtIntensity;
	};

	struct UniformBufferMaterial
	{
		glm::vec3 DiffuseColor;
		float Metalness;
		float Roughness;
		float Emission;
		float Transparency;
		float EnvMapRotation;
		
		int32 UseNormalMap;
		int32 Padding1;
	};
}

