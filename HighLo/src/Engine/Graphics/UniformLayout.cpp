#include "HighLoPch.h"
#include "UniformLayout.h"

namespace highlo
{
	UniformLayout UniformLayout::GetCameraLayout()
	{
		return {
			{ "u_Camera.ViewProjectionMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.InverseViewProjectionMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.ProjectionMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.InverseProjectionMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.ViewMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.InverseViewMatrix", UniformLayoutDataType::Mat4 },
			{ "u_Camera.NDCToViewMul", UniformLayoutDataType::Float2 },
			{ "u_Camera.NDCToViewAdd", UniformLayoutDataType::Float2 },
			{ "u_Camera.DepthUnpackConsts", UniformLayoutDataType::Float2 },
			{ "u_Camera.CameraTanHalfFOV", UniformLayoutDataType::Float2 },
		};
	}

	UniformLayout UniformLayout::GetScreenDataLayout()
	{
		return {
			{ "u_ScreenData.InvFullResolution", UniformLayoutDataType::Float2 },
			{ "u_ScreenData.FullResolution", UniformLayoutDataType::Float2 },
			{ "u_ScreenData.InvHalfResolution", UniformLayoutDataType::Float2 },
			{ "u_ScreenData.HalfResolution", UniformLayoutDataType::Float2 },
		};
	}

	UniformLayout UniformLayout::GetRendererDataLayout()
	{
		return {
			{ "u_RendererData.CascadeSplits", UniformLayoutDataType::Float4 },
			{ "u_RendererData.LightSize", UniformLayoutDataType::Float },
			{ "u_RendererData.MaxShadowDistance", UniformLayoutDataType::Float },
			{ "u_RendererData.ShadowFade", UniformLayoutDataType::Float },
			{ "u_RendererData.CascadeTransitionFade", UniformLayoutDataType::Float },
			{ "u_RendererData.TilesCountX", UniformLayoutDataType::Int },
			{ "u_RendererData.ShowLightComplexity", UniformLayoutDataType::Bool },
			{ "u_RendererData.ShowCascades", UniformLayoutDataType::Bool },
			{ "u_RendererData.SoftShadows", UniformLayoutDataType::Bool },
			{ "u_RendererData.CascadeFading", UniformLayoutDataType::Bool },
		};
	}

	UniformLayout UniformLayout::GetHBAODataLayout()
	{
		return {
			{ "u_HBAO.Float2Offsets", UniformLayoutDataType::Float4, 16 },
			{ "u_HBAO.Jitters", UniformLayoutDataType::Float4, 16 },
			{ "u_HBAO.PerspectiveInfo", UniformLayoutDataType::Float4 },
			{ "u_HBAO.RadiusToScreen", UniformLayoutDataType::Float },
			{ "u_HBAO.NegInvR2", UniformLayoutDataType::Float },
			{ "u_HBAO.NDotVBias", UniformLayoutDataType::Float },
			{ "u_HBAO.AOMultiplier", UniformLayoutDataType::Float },
			{ "u_HBAO.PowExponent", UniformLayoutDataType::Float },
			{ "u_HBAO.Padding1", UniformLayoutDataType::Float3 },
			{ "u_HBAO.ShadowTolerance", UniformLayoutDataType::Float },
			{ "u_HBAO.InvQuarterResolution", UniformLayoutDataType::Float2 },
			{ "u_HBAO.Padding2", UniformLayoutDataType::Float2 },
			{ "u_HBAO.IsOrtho", UniformLayoutDataType::Bool },
		};
	}

	UniformLayout UniformLayout::GetShadowDataLayout()
	{
		return {
			{ "u_DirShadow.DirLightMatrices", UniformLayoutDataType::Mat4, 4 },
		};
	}

	UniformLayout UniformLayout::GetSceneDataLayout()
	{
		return {
			// this is a custom struct, so insert only bytesize (or in this case the struct is exactly 2 * vec4 bytes)
			{ "u_Scene.DirectionalLights", UniformLayoutDataType::Float4, 2 },

			{ "u_Scene.CameraPosition", UniformLayoutDataType::Float3 },
			{ "u_Scene.EnvironmentMapIntensity", UniformLayoutDataType::Float },
		};
	}

	UniformLayout UniformLayout::GetPointLightDataLayout()
	{
		return {
			// TODO:											make this uint
			{ "u_PointLights.LightCount", UniformLayoutDataType::Int },
			{ "u_PointLights.Lights", UniformLayoutDataType::Float4, 3 }, // again custom struct, but this time it is 3 * vec4
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

