// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UniformLayout.h"

namespace highlo
{
	std::vector<UniformVariable> UniformLayout::GetCameraLayout()
	{
		return {
			{ "u_Camera.ViewProjectionMatrix", UniformLayoutDataType::Mat4, 1, 0 },
			{ "u_Camera.InverseViewProjectionMatrix", UniformLayoutDataType::Mat4, 1, sizeof(glm::mat4)},
			{ "u_Camera.ProjectionMatrix", UniformLayoutDataType::Mat4, 1, 2 * sizeof(glm::mat4) },
			{ "u_Camera.InverseProjectionMatrix", UniformLayoutDataType::Mat4, 1, 3 * sizeof(glm::mat4)},
			{ "u_Camera.ViewMatrix", UniformLayoutDataType::Mat4, 1, 4 * sizeof(glm::mat4) },
			{ "u_Camera.InverseViewMatrix", UniformLayoutDataType::Mat4, 1, 5 * sizeof(glm::mat4) },
			{ "u_Camera.NDCToViewMul", UniformLayoutDataType::Float2, 1, 6 * sizeof(glm::mat4) },
			{ "u_Camera.NDCToViewAdd", UniformLayoutDataType::Float2, 1, 6 * sizeof(glm::mat4) + sizeof(glm::vec2) },
			{ "u_Camera.DepthUnpackConsts", UniformLayoutDataType::Float2, 1, 6 * sizeof(glm::mat4) + 2 * sizeof(glm::vec2) },
			{ "u_Camera.CameraTanHalfFOV", UniformLayoutDataType::Float2, 1, 6 * sizeof(glm::mat4) + 3 * sizeof(glm::vec2) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetScreenDataLayout()
	{
		return {
			{ "u_ScreenData.InvFullResolution", UniformLayoutDataType::Float2, 1, 0 },
			{ "u_ScreenData.FullResolution", UniformLayoutDataType::Float2, 1, sizeof(glm::vec2) },
			{ "u_ScreenData.InvHalfResolution", UniformLayoutDataType::Float2, 1, 2 * sizeof(glm::vec2) },
			{ "u_ScreenData.HalfResolution", UniformLayoutDataType::Float2, 1, 3 * sizeof(glm::vec2) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetRendererDataLayout()
	{
		return {
			{ "u_RendererData.CascadeSplits", UniformLayoutDataType::Float4, 1, 0 },
			{ "u_RendererData.LightSize", UniformLayoutDataType::Float, 1, sizeof(glm::vec4) },
			{ "u_RendererData.MaxShadowDistance", UniformLayoutDataType::Float, 1, sizeof(glm::vec4) + sizeof(float) },
			{ "u_RendererData.ShadowFade", UniformLayoutDataType::Float, 1, sizeof(glm::vec4) + 2 * sizeof(float) },
			{ "u_RendererData.CascadeTransitionFade", UniformLayoutDataType::Float, 1, sizeof(glm::vec4) + 3 * sizeof(float) },
			{ "u_RendererData.TilesCountX", UniformLayoutDataType::Int, 1, sizeof(glm::vec4) + 4 * sizeof(float) },
			{ "u_RendererData.ShowLightComplexity", UniformLayoutDataType::Bool, 1, sizeof(glm::vec4) + 4 * sizeof(float) + sizeof(int32) },
			{ "u_RendererData.ShowCascades", UniformLayoutDataType::Bool, 1, sizeof(glm::vec4) + 4 * sizeof(float) + sizeof(int32) + sizeof(bool) },
			{ "u_RendererData.SoftShadows", UniformLayoutDataType::Bool, 1, sizeof(glm::vec4) + 4 * sizeof(float) + sizeof(int32) + 2 * sizeof(bool) },
			{ "u_RendererData.CascadeFading", UniformLayoutDataType::Bool, 1, sizeof(glm::vec4) + 4 * sizeof(float) + sizeof(int32) + 3 * sizeof(bool) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetHBAODataLayout()
	{
		return {
			{ "u_HBAO.Float2Offsets", UniformLayoutDataType::Float4, 16, 0 },
			{ "u_HBAO.Jitters", UniformLayoutDataType::Float4, 16, 16 * sizeof(glm::vec4) },
			{ "u_HBAO.PerspectiveInfo", UniformLayoutDataType::Float4, 1, 32 * sizeof(glm::vec4) },
			{ "u_HBAO.RadiusToScreen", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) },
			{ "u_HBAO.NegInvR2", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) + sizeof(float) },
			{ "u_HBAO.NDotVBias", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) + 2 * sizeof(float) },
			{ "u_HBAO.AOMultiplier", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) + 3 * sizeof(float) },
			{ "u_HBAO.PowExponent", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) + 4 * sizeof(float) },
			{ "u_HBAO.Padding1", UniformLayoutDataType::Float3, 1, 33 * sizeof(glm::vec4) + 5 * sizeof(float)},
			{ "u_HBAO.ShadowTolerance", UniformLayoutDataType::Float, 1, 33 * sizeof(glm::vec4) + 5 * sizeof(float) + sizeof(glm::vec3) },
			{ "u_HBAO.InvQuarterResolution", UniformLayoutDataType::Float2, 1, 33 * sizeof(glm::vec4) + 6 * sizeof(float) + sizeof(glm::vec3) },
			{ "u_HBAO.Padding2", UniformLayoutDataType::Float2, 1, 33 * sizeof(glm::vec4) + 6 * sizeof(float) + sizeof(glm::vec3) + sizeof(glm::vec2) },
			{ "u_HBAO.IsOrtho", UniformLayoutDataType::Bool, 1, 33 * sizeof(glm::vec4) + 6 * sizeof(float) + sizeof(glm::vec3) + 2 * sizeof(glm::vec2) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetShadowDataLayout()
	{
		return {
			{ "u_DirShadow.DirLightMatrices", UniformLayoutDataType::Mat4, 4, 0 },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetSceneDataLayout()
	{
		return {
			// this is a custom struct, so insert only bytesize (or in this case the struct is exactly 2 * vec4 bytes)
			{ "u_Scene.DirectionalLights", UniformLayoutDataType::Float4, 2, 0 },

			{ "u_Scene.CameraPosition", UniformLayoutDataType::Float3, 1, 2 * sizeof(glm::vec4) },
			{ "u_Scene.EnvironmentMapIntensity", UniformLayoutDataType::Float, 1, 2 * sizeof(glm::vec3) + sizeof(glm::vec3) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetSceneCompositeLayout()
	{
		return {
			{ "u_Uniforms.Exposure", UniformLayoutDataType::Float, 1, 0 },
			{ "u_Uniforms.BloomIntensity", UniformLayoutDataType::Float, 1, sizeof(float) },
			{ "u_Uniforms.BloomDirtIntensity", UniformLayoutDataType::Float, 1, 2 * sizeof(float) },
		};
	}

	std::vector<UniformVariable> UniformLayout::GetPointLightDataLayout()
	{
		return {
			// TODO:											make this uint
			{ "u_PointLights.LightCount", UniformLayoutDataType::Int, 1, 0 },
			{ "u_PointLights.Lights", UniformLayoutDataType::Float4, 3, sizeof(uint32) }, // again custom struct, but this time it is 3 * vec4
		};
	}

	std::vector<HLString> UniformLayout::GetNames()
	{
		std::vector<HLString> result;

		for (auto &element : m_Elements)
		{
			result.push_back(element.Name);
		}

		return result;
	}

	void UniformLayout::CalculateOffsetsAndStride()
	{
		uint32 offset = 0;
		m_Stride = 0;
		for (auto &element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}

