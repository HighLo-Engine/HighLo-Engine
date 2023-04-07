#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

struct VertexOutput
{
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;

	mat3 CameraView; 

	vec4 ShadowMapCoords[4];
	vec3 ViewPosition;
};

layout(location = 0) out VertexOutput Output;

invariant gl_Position;

void main()
{
	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
	);

	vec4 worldPosition = transform * vec4(a_Position, 1.0);

	Output.WorldPosition = worldPosition.xyz;
	Output.Normal = mat3(transform) * a_Normal;
	Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	Output.WorldNormals = mat3(transform) * mat3(a_Tangent, a_Binormal, a_Normal);
	Output.WorldTransform = mat3(transform);
	Output.Binormal = a_Binormal;

	Output.CameraView = mat3(u_Camera.ViewMatrix);

	Output.ShadowMapCoords[0] = u_DirShadow.DirLightMatrices[0] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[1] = u_DirShadow.DirLightMatrices[1] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[2] = u_DirShadow.DirLightMatrices[2] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[3] = u_DirShadow.DirLightMatrices[3] * vec4(Output.WorldPosition, 1.0);
	Output.ViewPosition = vec3(u_Camera.ViewMatrix * vec4(Output.WorldPosition, 1.0));

	gl_Position = u_Camera.ViewProjectionMatrix * worldPosition;
}

#version 450 core 
#pragma shader:fragment 

#include <Buffers.glslh>
#include <PBR.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

struct VertexOutput
{
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;

	mat3 CameraView;

	vec4 ShadowMapCoords[4];
	vec3 ViewPosition;
};
 
layout(location = 0) in VertexOutput Input;

layout(location = 0) out vec4 o_Color;

#ifdef __VULKAN__
	layout(push_constant) uniform Material
	{
		vec3 DiffuseColor;
	//	float Metalness;
	//	float Roughness;
	//	float Emission;
	//	float Transparency;
	//	float EnvMapRotation;
	//
	//	bool UseNormalMap;
	//	bool Padding1;
	} u_MaterialUniforms;
#else
	layout(std140, binding = 13) uniform Material
	{
		vec3 DiffuseColor;
	//	float Roughness;
	//	float Metalness;
	//	float Emission;
	//	float Transparency;
	//	float EnvMapRotation;
	//
	//	bool UseNormalMap;
	//	bool Padding1;
	} u_MaterialUniforms;
#endif

#ifdef __VULKAN__
	// PBR texture inputs
	layout(set = 0, binding = 5) uniform sampler2D u_DiffuseTexture;
	layout(set = 0, binding = 6) uniform sampler2D u_NormalTexture;
	layout(set = 0, binding = 7) uniform sampler2D u_MetalnessTexture;
	layout(set = 0, binding = 8) uniform sampler2D u_RoughnessTexture;

	// Environment maps
	layout(set = 1, binding = 9) uniform samplerCube u_EnvRadianceTex;
	layout(set = 1, binding = 10) uniform samplerCube u_EnvIrradianceTex;

	// BRDF LUT
	layout(set = 1, binding = 11) uniform sampler2D u_BRDFLUTTexture;

	// Shadow maps
	layout(set = 1, binding = 12) uniform sampler2DArray u_ShadowMapTexture;
	layout(set = 1, binding = 21) uniform sampler2D u_SpotShadowTexture;
#else
	// PBR texture inputs
	layout(binding = 5) uniform sampler2D u_DiffuseTexture;
	layout(binding = 6) uniform sampler2D u_NormalTexture;
	layout(binding = 7) uniform sampler2D u_MetalnessTexture;
	layout(binding = 8) uniform sampler2D u_RoughnessTexture;

	// Environment maps
	layout(binding = 9) uniform samplerCube u_EnvRadianceTex;
	layout(binding = 10) uniform samplerCube u_EnvIrradianceTex;

	// BRDF LUT
	layout(binding = 11) uniform sampler2D u_BRDFLUTTexture;

	// TODO
	// Shadow maps
	//layout(binding = 12) uniform sampler2DArray u_ShadowMapTexture;
	//layout(binding = 21) uniform sampler2D u_SpotShadowTexture;
#endif

void main()
{
//	// Standard PBR inputs
//	vec4 diffuseTexColor = texture(u_DiffuseTexture, Input.TexCoord);
//	m_Params.Albedo = diffuseTexColor .rgb * u_MaterialUniforms.DiffuseColor;
//	float alpha = diffuseTexColor .a;
//	m_Params.Metalness = 0.0f;
//	m_Params.Roughness = 0.0f;
//	m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight
//
//	// Normals (either from vertex or map)
//	m_Params.Normal = normalize(Input.Normal);
//
//	m_Params.View = normalize(u_Scene.CameraPosition - Input.WorldPosition);
//	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);
//
//	// Specular reflection vector
//	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;
//
//	// Fresnel reflectance, metals use diffuse
//	vec3 F0 = mix(Fdielectric, m_Params.Diffuse, m_Params.Metalness);
//
//	uint cascadeIndex = 0;
//
//	const uint SHADOW_MAP_CASCADE_COUNT = 4;
//	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
//	{
//		if (Input.ViewPosition.z < u_RendererData.CascadeSplits[i])
//			cascadeIndex = i + 1;
//	}
//
//	float shadowDistance = u_RendererData.MaxShadowDistance;//u_CascadeSplits[3];
//	float transitionDistance = u_RendererData.ShadowFade;
//	float distance = length(Input.ViewPosition);
//	ShadowFade = distance - (shadowDistance - transitionDistance);
//	ShadowFade /= transitionDistance;
//	ShadowFade = clamp(1.0 - ShadowFade, 0.0, 1.0);
//
//	float shadowScale;
//
//	bool fadeCascades = u_RendererData.CascadeFading;
//	if (fadeCascades)
//	{
//		float cascadeTransitionFade = u_RendererData.CascadeTransitionFade;
//
//		float c0 = smoothstep(u_RendererData.CascadeSplits[0] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[0] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
//		float c1 = smoothstep(u_RendererData.CascadeSplits[1] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[1] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
//		float c2 = smoothstep(u_RendererData.CascadeSplits[2] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[2] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
//		if (c0 > 0.0 && c0 < 1.0)
//		{
//			// Sample 0 & 1
//			vec3 shadowMapCoords = (Input.ShadowMapCoords[0].xyz / Input.ShadowMapCoords[0].w);
//			float shadowAmount0 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 0, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 0, shadowMapCoords);
//			shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
//			float shadowAmount1 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 1, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 1, shadowMapCoords);
//
//			shadowScale = mix(shadowAmount0, shadowAmount1, c0);
//		}
//		else if (c1 > 0.0 && c1 < 1.0)
//		{
//			// Sample 1 & 2
//			vec3 shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
//			float shadowAmount1 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 1, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 1, shadowMapCoords);
//			shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
//			float shadowAmount2 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 2, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 2, shadowMapCoords);
//
//			shadowScale = mix(shadowAmount1, shadowAmount2, c1);
//		}
//		else if (c2 > 0.0 && c2 < 1.0)
//		{
//			// Sample 2 & 3
//			vec3 shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
//			float shadowAmount2 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 2, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 2, shadowMapCoords);
//			shadowMapCoords = (Input.ShadowMapCoords[3].xyz / Input.ShadowMapCoords[3].w);
//			float shadowAmount3 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, 3, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, 3, shadowMapCoords);
//
//			shadowScale = mix(shadowAmount2, shadowAmount3, c2);
//		}
//		else
//		{
//			vec3 shadowMapCoords = (Input.ShadowMapCoords[cascadeIndex].xyz / Input.ShadowMapCoords[cascadeIndex].w);
//			shadowScale = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, cascadeIndex, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, cascadeIndex, shadowMapCoords);
//		}
//	}
//	else
//	{
//		vec3 shadowMapCoords = (Input.ShadowMapCoords[cascadeIndex].xyz / Input.ShadowMapCoords[cascadeIndex].w);
//		shadowScale = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, cascadeIndex, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, cascadeIndex, shadowMapCoords);
//	}
//
//	shadowScale = 1.0 - clamp(u_Scene.DirectionalLights.ShadowAmount - shadowScale, 0.0f, 1.0f);
//
//	vec3 lightContribution = CalculateDirLights(F0) * shadowScale;
//	lightContribution += CalculatePointLights(F0, Input.WorldPosition);
//	lightContribution += m_Params.Albedo * u_MaterialUniforms.Emission;
//	vec3 iblContribution = IBL(F0, Lr, u_EnvRadianceTex, u_EnvIrradianceTex, u_BRDFLUTTexture, u_MaterialUniforms.EnvMapRotation, m_Params.NdotV, m_Params.Roughness, m_Params.Metalness, m_Params.Diffuse, m_Params.Normal, m_Params.View);
//	iblContribution *= u_Scene.EnvironmentMapIntensity;
//
//	//color = vec4(iblContribution + lightContribution, 1.0);
//	color = vec4(m_Params.Albedo, u_MaterialUniforms.Transparency);
//
//	// TODO: Temporary bug fix.
//	if (u_Scene.DirectionalLights.Multiplier <= 0.0f)
//		shadowScale = 0.0f;
//	 
//	if (u_RendererData.ShowLightComplexity)
//	{
//		int pointLightCount = GetPointLightCount();
//		float value = float(pointLightCount);
//		color.rgb = (color.rgb * 0.2) + GetGradient(value);
//	}
//	
//	if (u_RendererData.ShowCascades)
//	{
//		switch (cascadeIndex)
//		{
//		case 0:
//			color.rgb *= vec3(1.0f, 0.25f, 0.25f);
//			break;
//		case 1:
//			color.rgb *= vec3(0.25f, 1.0f, 0.25f);
//			break;
//		case 2:
//			color.rgb *= vec3(0.25f, 0.25f, 1.0f);
//			break;
//		case 3:
//			color.rgb *= vec3(1.0f, 1.0f, 0.25f);
//			break;
//		}
//	}

	// TEMP
	o_Color = vec4(u_MaterialUniforms.DiffuseColor, 1.0);
	vec4 tex = texture(u_DiffuseTexture, Input.TexCoord);
	vec4 normal = texture(u_NormalTexture, Input.TexCoord);
	vec4 metal = texture(u_MetalnessTexture, Input.TexCoord);
	vec4 roughness = texture(u_RoughnessTexture, Input.TexCoord);
	vec4 tmpEnvRadiance = texture(u_EnvRadianceTex, vec3(Input.TexCoord, 1.0));
	vec4 tmpEnvIrradiance = texture(u_EnvIrradianceTex, vec3(Input.TexCoord, 1.0));
	vec4 brdfMap = texture(u_BRDFLUTTexture, Input.TexCoord);
}

