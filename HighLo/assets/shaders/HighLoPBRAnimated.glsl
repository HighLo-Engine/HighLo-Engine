#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

const int MAX_BONES = 100;
const int MAX_ANIMATED_MESHES = 1024;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

layout(location = 5) in ivec4 a_BoneIndices;
layout(location = 6) in vec4 a_BoneWeights;

invariant gl_Position;

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

#ifdef __VULKAN__
layout(std140, set = 2, binding = 22) readonly buffer BoneTransforms
{
	mat4 BoneTransform[MAX_BONES * MAX_ANIMATED_MESHES];
} r_BoneTransforms;

layout(push_constant) uniform BoneTransformIndex
{
	uint Base;
} u_BoneTransformIndex;
#else
layout(binding = 22, std140) readonly buffer BoneTransforms
{
	mat4 BoneTransform[MAX_BONES * MAX_ANIMATED_MESHES];
} r_BoneTransforms;

layout(binding = 19, std140) uniform BoneTransformIndex
{
	uint Base;
} u_BoneTransformIndex;
#endif // __VULKAN__

void main()
{
	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
	);

#ifdef __VULKAN__
	int instanceIndex = gl_InstanceIndex;
#else
	int instanceIndex = gl_InstanceID;
#endif // __VULKAN__

	mat4 boneTransform = r_BoneTransforms.BoneTransform[(u_BoneTransformIndex.Base + instanceIndex) * MAX_BONES + a_BoneIndices[0]] * a_BoneWeights[0];
	boneTransform     += r_BoneTransforms.BoneTransform[(u_BoneTransformIndex.Base + instanceIndex) * MAX_BONES + a_BoneIndices[1]] * a_BoneWeights[1];
	boneTransform     += r_BoneTransforms.BoneTransform[(u_BoneTransformIndex.Base + instanceIndex) * MAX_BONES + a_BoneIndices[2]] * a_BoneWeights[2];
	boneTransform     += r_BoneTransforms.BoneTransform[(u_BoneTransformIndex.Base + instanceIndex) * MAX_BONES + a_BoneIndices[3]] * a_BoneWeights[3];

	vec4 worldPosition = transform * boneTransform * vec4(a_Position, 1.0);

	Output.WorldPosition = worldPosition.xyz;
	Output.Normal = mat3(transform) * mat3(boneTransform) * a_Normal;
	Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	Output.WorldNormals = mat3(transform) * mat3(boneTransform) * mat3(a_Tangent, a_Binormal, a_Normal);
	Output.WorldTransform = mat3(transform) * mat3(boneTransform);
	Output.Binormal = mat3(transform) * mat3(boneTransform) * a_Binormal;
	Output.CameraView = mat3(u_Camera.ViewMatrix);
	Output.ViewPosition = vec3(u_Camera.ViewMatrix * vec4(Output.WorldPosition, 1.0));

	Output.ShadowMapCoords[0] = u_DirShadow.DirLightMatrices[0] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[1] = u_DirShadow.DirLightMatrices[1] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[2] = u_DirShadow.DirLightMatrices[2] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[3] = u_DirShadow.DirLightMatrices[3] * vec4(Output.WorldPosition, 1.0);

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

// PBR texture inputs
layout(set = 0, binding = 5) uniform sampler2D u_DiffuseTexture;
layout(set = 0, binding = 6) uniform sampler2D u_NormalTexture;
layout(set = 0, binding = 7) uniform sampler2D u_MetalnessTexture;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessTexture;

// Environment maps
layout(binding = 9) uniform samplerCube u_EnvRadianceTex;
layout(binding = 10) uniform samplerCube u_EnvIrradianceTex;

// BRDF LUT
layout(binding = 11) uniform sampler2D u_BRDFLUTTexture;

// Shadow maps
layout(binding = 12) uniform sampler2DArray u_ShadowMapTexture;

#ifdef __VULKAN__
	layout(push_constant) uniform Material
	{
		vec3 DiffuseColor;
		float Metalness;
		float Roughness;
		float Emission;
		float Transparency;
		float EnvMapRotation;
	
		bool UseNormalMap;
		bool Padding1;
	} u_MaterialUniforms;
#else
	layout(std140, binding = 13) uniform Material
	{
		vec3 DiffuseColor;
		float Roughness;
		float Metalness;
		float Emission;
		float Transparency;
		float EnvMapRotation;
	
		bool UseNormalMap;
		bool Padding1;
	} u_MaterialUniforms;
#endif

vec3 IBL(vec3 F0, vec3 Lr)
{
	vec3 irradiance = texture(u_EnvIrradianceTex, m_Params.Normal).rgb;
	vec3 F = FresnelSchlickRoughness(F0, m_Params.NdotV, m_Params.Roughness);
	vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
	vec3 diffuseIBL = m_Params.Diffuse * irradiance;

	int envRadianceTexLevels = textureQueryLevels(u_EnvRadianceTex);
	float NoV = clamp(m_Params.NdotV, 0.0, 1.0);
	vec3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	vec3 specularIrradiance = textureLod(u_EnvRadianceTex, Rotate(u_MaterialUniforms.EnvMapRotation, Lr), (m_Params.Roughness) * envRadianceTexLevels).rgb;
	//specularIrradiance = vec3(Convert_sRGB_FromLinear(specularIrradiance.r), Convert_sRGB_FromLinear(specularIrradiance.g), Convert_sRGB_FromLinear(specularIrradiance.b));

	// Sample BRDF Lut, 1.0 - roughness for y-coord because texture was generated for gloss model
	vec2 specularBRDF = texture(u_BRDFLUTTexture, vec2(m_Params.NdotV, 1.0 - m_Params.Roughness)).rg;
	vec3 specularIBL = specularIrradiance * (F0 * specularBRDF.x + specularBRDF.y);

	return kd * diffuseIBL + specularIBL;
}

vec3 GetGradient(float value)
{
	vec3 zero = vec3(0.0, 0.0, 0.0);
	vec3 white = vec3(0.0, 0.1, 0.9);
	vec3 red = vec3(0.2, 0.9, 0.4);
	vec3 blue = vec3(0.8, 0.8, 0.3);
	vec3 green = vec3(0.9, 0.2, 0.3);

	float step0 = 0.0f;
	float step1 = 2.0f;
	float step2 = 4.0f;
	float step3 = 8.0f;
	float step4 = 16.0f;

	vec3 color = mix(zero, white, smoothstep(step0, step1, value));
	color = mix(color, white, smoothstep(step1, step2, value));
	color = mix(color, red, smoothstep(step1, step2, value));
	color = mix(color, blue, smoothstep(step2, step3, value));
	color = mix(color, green, smoothstep(step3, step4, value));

	return color;
}

void main()
{
	// Standard PBR inputs
	vec4 diffuseTexColor = texture(u_DiffuseTexture, Input.TexCoord);
	m_Params.Diffuse = diffuseTexColor.rgb * u_MaterialUniforms.DiffuseColor;
	float alpha = diffuseTexColor.a;
	m_Params.Metalness = texture(u_MetalnessTexture, Input.TexCoord).r * u_MaterialUniforms.Metalness;
	m_Params.Roughness = texture(u_RoughnessTexture, Input.TexCoord).r * u_MaterialUniforms.Roughness;
	m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight

	// Normals (either from vertex or map)
	m_Params.Normal = normalize(Input.Normal);
	if (u_MaterialUniforms.UseNormalMap)
	{
		m_Params.Normal = normalize(texture(u_NormalTexture, Input.TexCoord).rgb * 2.0f - 1.0f);
		m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
	}

	m_Params.View = normalize(u_Scene.CameraPosition - Input.WorldPosition);
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

	// Specular reflection vector
	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;

	// Fresnel reflectance, metals use diffuse
	vec3 F0 = mix(Fdielectric, m_Params.Diffuse, m_Params.Metalness);

	uint cascadeIndex = 0;

	const uint SHADOW_MAP_CASCADE_COUNT = 4;
	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
	{
		if (Input.ViewPosition.z < u_RendererData.CascadeSplits[i])
			cascadeIndex = i + 1;
	}

	float shadowDistance = u_RendererData.MaxShadowDistance; //u_CascadeSplits[3];
	float transitionDistance = u_RendererData.ShadowFade;
	float distance = length(Input.ViewPosition);
	float shadowFade = distance - (shadowDistance - transitionDistance);
	shadowFade /= transitionDistance;
	shadowFade = clamp(1.0 - shadowFade, 0.0, 1.0);

	float shadowScale;

	bool fadeCascades = u_RendererData.CascadeFading;
	if (fadeCascades)
	{
		float cascadeTransitionFade = u_RendererData.CascadeTransitionFade;

		float c0 = smoothstep(u_RendererData.CascadeSplits[0] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[0] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c1 = smoothstep(u_RendererData.CascadeSplits[1] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[1] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c2 = smoothstep(u_RendererData.CascadeSplits[2] + cascadeTransitionFade * 0.5f, u_RendererData.CascadeSplits[2] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		
		if (c0 > 0.0 && c0 < 1.0)
		{
			// Sample 0 & 1
			vec3 shadowMapCoords = (Input.ShadowMapCoords[0].xyz / Input.ShadowMapCoords[0].w);
			float shadowAmount0 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 0, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 0, shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
			float shadowAmount1 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 1, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 1, shadowMapCoords);

			shadowScale = mix(shadowAmount0, shadowAmount1, c0);
		}
		else if (c1 > 0.0 && c1 < 1.0)
		{
			// Sample 1 & 2
			vec3 shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
			float shadowAmount1 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 1, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 1, shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
			float shadowAmount2 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 2, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 2, shadowMapCoords);

			shadowScale = mix(shadowAmount1, shadowAmount2, c1);
		}
		else if (c2 > 0.0 && c2 < 1.0)
		{
			// Sample 2 & 3
			vec3 shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
			float shadowAmount2 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 2, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 2, shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[3].xyz / Input.ShadowMapCoords[3].w);
			float shadowAmount3 = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, 3, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, 3, shadowMapCoords);

			shadowScale = mix(shadowAmount2, shadowAmount3, c2);
		}
		else
		{
			vec3 shadowMapCoords = (Input.ShadowMapCoords[cascadeIndex].xyz / Input.ShadowMapCoords[cascadeIndex].w);
			shadowScale = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, cascadeIndex, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, cascadeIndex, shadowMapCoords);
		}
	}
	else
	{
		vec3 shadowMapCoords = (Input.ShadowMapCoords[cascadeIndex].xyz / Input.ShadowMapCoords[cascadeIndex].w);
		shadowScale = u_RendererData.SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture, shadowFade, cascadeIndex, shadowMapCoords, u_RendererData.LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture, u_RendererData.ShadowFade, cascadeIndex, shadowMapCoords);
	}

	shadowScale = 1.0 - clamp(u_Scene.DirectionalLights.ShadowAmount - shadowScale, 0.0f, 1.0f);

	// Direct lighting
	vec3 lightContribution = CalculateDirLights(F0) * shadowScale;
	lightContribution += CalculatePointLights(F0, Input.WorldPosition);
	//lightContribution += CalculateSpotLights(F0, Input.WorldPosition) * SpotShadowCalculation(u_SpotShadowTexture, Input.WorldPosition);
	lightContribution += m_Params.Diffuse * u_MaterialUniforms.Emission;

	// Indirect lighting
	vec3 iblContribution = IBL(F0, Lr) * u_Scene.EnvironmentMapIntensity;

	o_Color = vec4(iblContribution + lightContribution, 1.0);
}

