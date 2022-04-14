// HighLoPBR Animated Shader
// 
// References:
// - Unreal Engine 4 PBR Notes (https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf)
// - Frostbite's SIGGRAPH paper from 2014 (https://seblagarde.wordpress.com/2015/07/14/siggraph-2014-moving-frostbite-to-physically-based-rendering/)
// - Michał Siejak's PBR Project (https://github.com/Nadrin)
// - Sparky Engine (https://github.com/TheCherno/Sparky)

#version 430 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

// Animation specific
layout(location = 5) in ivec4 a_BoneIndices;
layout(location = 6) in vec4 a_BoneWeights;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
	mat4 u_ViewMatrix;
};

layout(std140, binding = 1) uniform ShadowData
{
	mat4 u_LightMatrix[4];
};

layout(std140, binding = 2) uniform Transform
{
	mat4 u_Transform;
	mat4 u_BoneTransforms[100];
};

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

void main()
{
	mat4 boneTransform = u_BoneTransforms[a_BoneIndices.x] * a_BoneWeights.x;
	boneTransform += u_BoneTransforms[a_BoneIndices.y] * a_BoneWeights.y;
	boneTransform += u_BoneTransforms[a_BoneIndices.z] * a_BoneWeights.z;
	boneTransform += u_BoneTransforms[a_BoneIndices.w] * a_BoneWeights.w;

	vec4 localPosition = boneTransform * vec4(a_Position, 1.0);

	Output.WorldPosition = vec3(u_Transform * boneTransform * vec4(a_Position, 1.0));
	Output.Normal = mat3(u_Transform) * mat3(boneTransform) * a_Normal;
	Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
	Output.Binormal = mat3(boneTransform) * a_Binormal;
	Output.CameraView = mat3(u_ViewMatrix);
	Output.ViewPosition = vec3(u_ViewMatrix * vec4(Output.WorldPosition, 1.0));

	Output.ShadowMapCoords[0] = u_LightMatrix[0] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[1] = u_LightMatrix[1] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[2] = u_LightMatrix[2] * vec4(Output.WorldPosition, 1.0);
	Output.ShadowMapCoords[3] = u_LightMatrix[3] * vec4(Output.WorldPosition, 1.0);

	gl_Position = u_ViewProjectionMatrix * u_Transform * localPosition;
}

#version 430 core
#pragma shader:fragment

const float PI = 3.141592;
const float Epsilon = 0.00001;
const int LightCount = 1;

uint CascadeIndex = 0;
float ShadowFade = 1.0;

float u_Light_zNear = 0.0; // 0.01 gives artifacts? maybe because of ortho proj?
float u_Light_zFar = 10000.0;
vec2 u_LightRadiusUV = vec2(0.05);

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

const vec2 PoissonDistribution[64] = vec2[](
	vec2(-0.884081, 0.124488),
	vec2(-0.714377, 0.027940),
	vec2(-0.747945, 0.227922),
	vec2(-0.939609, 0.243634),
	vec2(-0.985465, 0.045534),
	vec2(-0.861367, -0.136222),
	vec2(-0.881934, 0.396908),
	vec2(-0.466938, 0.014526),
	vec2(-0.558207, 0.212662),
	vec2(-0.578447, -0.095822),
	vec2(-0.740266, -0.095631),
	vec2(-0.751681, 0.472604),
	vec2(-0.553147, -0.243177),
	vec2(-0.674762, -0.330730),
	vec2(-0.402765, -0.122087),
	vec2(-0.319776, -0.312166),
	vec2(-0.413923, -0.439757),
	vec2(-0.979153, -0.201245),
	vec2(-0.865579, -0.288695),
	vec2(-0.243704, -0.186378),
	vec2(-0.294920, -0.055748),
	vec2(-0.604452, -0.544251),
	vec2(-0.418056, -0.587679),
	vec2(-0.549156, -0.415877),
	vec2(-0.238080, -0.611761),
	vec2(-0.267004, -0.459702),
	vec2(-0.100006, -0.229116),
	vec2(-0.101928, -0.380382),
	vec2(-0.681467, -0.700773),
	vec2(-0.763488, -0.543386),
	vec2(-0.549030, -0.750749),
	vec2(-0.809045, -0.408738),
	vec2(-0.388134, -0.773448),
	vec2(-0.429392, -0.894892),
	vec2(-0.131597, 0.065058),
	vec2(-0.275002, 0.102922),
	vec2(-0.106117, -0.068327),
	vec2(-0.294586, -0.891515),
	vec2(-0.629418, 0.379387),
	vec2(-0.407257, 0.339748),
	vec2(0.071650, -0.384284),
	vec2(0.022018, -0.263793),
	vec2(0.003879, -0.136073),
	vec2(-0.137533, -0.767844),
	vec2(-0.050874, -0.906068),
	vec2(0.114133, -0.070053),
	vec2(0.163314, -0.217231),
	vec2(-0.100262, -0.587992),
	vec2(-0.004942, 0.125368),
	vec2(0.035302, -0.619310),
	vec2(0.195646, -0.459022),
	vec2(0.303969, -0.346362),
	vec2(-0.678118, 0.685099),
	vec2(-0.628418, 0.507978),
	vec2(-0.508473, 0.458753),
	vec2(0.032134, -0.782030),
	vec2(0.122595, 0.280353),
	vec2(-0.043643, 0.312119),
	vec2(0.132993, 0.085170),
	vec2(-0.192106, 0.285848),
	vec2(0.183621, -0.713242),
	vec2(0.265220, -0.596716),
	vec2(-0.009628, -0.483058),
	vec2(-0.018516, 0.435703)
);

struct DirectionalLight
{
	vec3 Direction;
	vec3 Radiance;
	float Multiplier;
};

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

layout(std140, binding = 3) uniform SceneData
{
	DirectionalLight u_DirectionalLights;
	vec3 u_CameraPosition;
	float u_EnvironmentMapIntensity;
};

layout(std140, binding = 4) uniform RendererData
{
	uniform vec4 u_CascadeSplits;
	uniform int u_TilesCountX;
	uniform bool u_ShowCascades;
	uniform bool u_SoftShadows;
	uniform bool u_CascadeFading;
	uniform bool u_ShowLightComplexity;
	uniform float u_LightSize;
	uniform float u_MaxShadowDistance;
	uniform float u_ShadowFade;
	uniform float u_CascadeTransitionFade;
	uniform float u_BloomThreshold;
	uniform float u_LightView;
};

layout(push_constant) uniform Material
{
	vec3 DiffuseColor;
	float Metalness;
	float Roughness;
	
	float EnvironmentMapRotation;

	// Toggles
	float DiffuseTextureToggle;
	float NormalTextureToggle;
	float MetalnessTextureToggle;
	float RoughnessTextureToggle;
} u_MaterialUniforms;

struct PBRParameters
{
	float Roughness;
	float Metalness;
	float NdotV;

	vec3 Diffuse;
	vec3 Normal;
	vec3 View;
};

PBRParameters m_Params;

layout(location = 0) in VertexOutput Input;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_BloomColor;

// PBR Texture Inputs
layout(set = 0, binding = 5) uniform sampler2D u_DiffuseTexture;
layout(set = 0, binding = 6) uniform sampler2D u_NormalTexture;
layout(set = 0, binding = 7) uniform sampler2D u_MetalnessTexture;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessTexture;

// Environment maps Inputs
layout(set = 1, binding = 9) uniform samplerCube u_EnvironmentRadianceTexture;
layout(set = 1, binding = 10) uniform samplerCube u_EnvironmentIrradianceTexture;
layout(set = 1, binding = 11) uniform sampler2D u_BRDFLUTTexture;

// Shadow maps
layout(binding = 12) uniform sampler2D u_ShadowMapTexture[32];

// Not used
// layout(binding = 13) uniform sampler2D u_LinearDepthTexture;

// =================================================================================================================================================
// =========================================== Base operations (By Disney and Schlick's algorithm) =================================================
// =================================================================================================================================================

// Normal distribution function, uses Disney's reparametrization of alpha = roughness^2
float ndfGGX(float cosLh, float roughness)
{
	float alpha = roughness * roughness;
	float alphaSquared = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSquared - 1.0) + 1.0;
	return alphaSquared / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float NdotV, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return gaSchlickG1(cosLi, k) * gaSchlickG1(NdotV, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// =================================================================================================================================================
// ========================================================== Light Calculations ===================================================================
// =================================================================================================================================================

vec3 RotateVectorAboutYAxis(float angle, vec3 axis)
{
	// Convert angle to radians
	angle = radians(angle);

	mat3x3 rotationMatrix =
	{
		vec3(cos(angle), 0.0, sin(angle)),
		vec3(0.0, 1.0, 0.0),
		vec3(-sin(angle), 0.0, cos(angle))
	};

	return rotationMatrix * axis;
}

vec3 Lighting(vec3 F0)
{
	vec3 result = vec3(0.0);
	for (int i = 0; i < LightCount; i++)
	{
		vec3 Li = u_DirectionalLights.Direction;
		vec3 Lradiance = u_DirectionalLights.Radiance * u_DirectionalLights.Multiplier;
		vec3 Lh = normalize(Li + m_Params.View);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(m_Params.Normal, Li));
		float cosLh = max(0.0, dot(m_Params.Normal, Lh));

		vec3 F = fresnelSchlick(F0, max(0.0, dot(Lh, m_Params.View)));
		float D = ndfGGX(cosLh, m_Params.Roughness);
		float G = gaSchlickGGX(cosLi, m_Params.NdotV, m_Params.Roughness);

		vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
		vec3 diffuseBRDF = kd * m_Params.Diffuse;
		vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * m_Params.NdotV);

		result += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	}

	return result;
}

vec3 IBL(vec3 F0, vec3 Lr)
{
	vec3 irradiance = texture(u_EnvironmentIrradianceTexture, m_Params.Normal).rgb;
	vec3 F = fresnelSchlickRoughness(F0, m_Params.NdotV, m_Params.Roughness);
	vec3 kd = (1.0 - F) * (1.0 - m_Params.Metalness);
	vec3 diffuseIBL = m_Params.Diffuse * irradiance;

	int envRadianceTextureLevels = textureQueryLevels(u_EnvironmentRadianceTexture);
	float NoV = clamp(m_Params.NdotV, 0.0, 1.0);
	vec3 R = 2.0 * dot(m_Params.View, m_Params.Normal) * m_Params.Normal - m_Params.View;
	vec3 specularIrradiance = textureLod(u_EnvironmentRadianceTexture, RotateVectorAboutYAxis(u_MaterialUniforms.EnvironmentMapRotation, Lr), (m_Params.Roughness) * envRadianceTextureLevels).rgb;

	vec2 specularBRDF = texture(u_BRDFLUTTexture, vec2(m_Params.NdotV, 1.0 - m_Params.Roughness)).rg;
	vec3 specularIBL = specularIrradiance * (F * specularBRDF.x + specularBRDF.y);

	return kd * diffuseIBL + specularIBL;
}

// =================================================================================================================================================
// ========================================================== Shadow Calculations / PCSS ===========================================================
// =================================================================================================================================================

vec2 SamplePoisson(int index)
{
	return PoissonDistribution[index % 64];
}

vec2 searchRegionRadiusUV(float zWorld)
{
	return u_LightRadiusUV * (zWorld - u_Light_zNear) / zWorld;
}

float GetShadowBias()
{
	const float MINIMUM_SHADOW_BIAS = 0.002;
	return max(MINIMUM_SHADOW_BIAS * (1.0 - dot(m_Params.Normal, u_DirectionalLights.Direction)), MINIMUM_SHADOW_BIAS);
}

float FindBlockerDistance_DirectionalLight(sampler2D shadowMap, vec3 shadowCoords, float uvLightSize)
{
	float bias = GetShadowBias();

	int numBlockerSearchSamples = 64;
	int blockers = 0;
	float avgBlockerDistance = 0.0;

	float zEye = -(u_LightView * vec4(Input.WorldPosition, 1.0)).z;
	vec2 searchWidth = searchRegionRadiusUV(zEye);

	for (int i = 0; i < numBlockerSearchSamples; i++)
	{
		float z = texture(shadowMap, shadowCoords.xy + SamplePoisson(i) * searchWidth).r;
		if (z < (shadowCoords.z - bias))
		{
			blockers++;
			avgBlockerDistance += z;
		}
	}

	if (blockers > 0)
		return avgBlockerDistance / float(blockers);

	return -1;
}

float HardShadows_DirectionalLight(sampler2D shadowMap, vec3 shadowCoords)
{
	float bias = GetShadowBias();
	float shadowMapDepth = texture(shadowMap, shadowCoords.xy).x;
	return 1.0 - step(shadowCoords.z, shadowMapDepth + bias) * ShadowFade;
}

float PCF_DirectionalLight(sampler2D shadowMap, vec3 shadowCoords, float uvRadius)
{
	float bias = GetShadowBias();
	int numPCFSamples = 64;

	float sum = 0;
	for (int i = 0; i < numPCFSamples; i++)
	{
		float z = texture(shadowMap, shadowCoords.xy + SamplePoisson(i) * uvRadius).r;
		sum += (z < (shadowCoords.z - bias)) ? 1 : 0;
	}

	return sum / numPCFSamples;
}

float PCSS_DirectionalLight(sampler2D shadowMap, vec3 shadowCoords, float uvLightSize)
{
	float blockerDistance = FindBlockerDistance_DirectionalLight(shadowMap, shadowCoords, uvLightSize);
	if (blockerDistance == -1)
		return 1;

	float penumbraWidth = (shadowCoords.z - blockerDistance) / blockerDistance;
	float NEAR = 0.01;

	float uvRadius = penumbraWidth * uvLightSize * NEAR / shadowCoords.z;
	return 1.0 - PCF_DirectionalLight(shadowMap, shadowCoords, uvRadius) * ShadowFade;
}

void main()
{
	m_Params.Diffuse = (u_MaterialUniforms.DiffuseTextureToggle > 0.5) ? texture(u_DiffuseTexture, Input.TexCoord).rgb : u_MaterialUniforms.DiffuseColor;
	m_Params.Metalness = (u_MaterialUniforms.MetalnessTextureToggle > 0.5) ? texture(u_MetalnessTexture, Input.TexCoord).r : u_MaterialUniforms.Metalness;
	m_Params.Roughness = (u_MaterialUniforms.RoughnessTextureToggle > 0.5) ? texture(u_RoughnessTexture, Input.TexCoord).r : u_MaterialUniforms.Roughness;
	m_Params.Roughness = max(m_Params.Roughness, 0.05); // minimum roughness to keep specular highlights
	
	m_Params.Normal = normalize(Input.Normal);
	if (u_MaterialUniforms.NormalTextureToggle > 0.5)
	{
		m_Params.Normal = normalize(2.0 * texture(u_NormalTexture, Input.TexCoord).rgb - 1.0);
		m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
	}

	m_Params.View = normalize(u_CameraPosition - Input.WorldPosition);
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

	// Specular reflection vector
	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;

	// Fresnel reflectance
	vec3 F0 = mix(Fdielectric, m_Params.Diffuse, m_Params.Metalness);

	const uint SHADOW_MAP_CASCADE_COUNT = 4;
	for (uint i = 0; i < SHADOW_MAP_CASCADE_COUNT - 1; i++)
	{
		if (Input.ViewPosition.z < u_CascadeSplits[i])
			CascadeIndex = i + 1;
	}

	// shadows
	float shadowDistance = u_MaxShadowDistance;
	float transitionDistance = u_ShadowFade;
	float distance = length(Input.ViewPosition);

	ShadowFade = distance - (shadowDistance - transitionDistance);
	ShadowFade /= transitionDistance;
	ShadowFade = clamp(1.0 - ShadowFade, 0.0, 1.0);

	bool fadeCascades = u_CascadeFading;
	float shadowAmount = 1.0;

	if (fadeCascades)
	{
		float cascadeTransitionFade = u_CascadeTransitionFade;

		float c0 = smoothstep(u_CascadeSplits[0] + cascadeTransitionFade * 0.5f, u_CascadeSplits[0] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c1 = smoothstep(u_CascadeSplits[1] + cascadeTransitionFade * 0.5f, u_CascadeSplits[1] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);
		float c2 = smoothstep(u_CascadeSplits[2] + cascadeTransitionFade * 0.5f, u_CascadeSplits[2] - cascadeTransitionFade * 0.5f, Input.ViewPosition.z);

		if (c0 > 0.0 && c0 < 1.0)
		{
			// Sample 0 & 1
			vec3 shadowMapCoords = (Input.ShadowMapCoords[0].xyz / Input.ShadowMapCoords[0].w);
			float shadow0 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[0], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[0], shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
			float shadow1 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[1], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[1], shadowMapCoords);

			shadowAmount = mix(shadow0, shadow1, c0);
		}
		else if (c1 > 0.0 && c1 < 1.0)
		{
			// Sample 1 & 2
			vec3 shadowMapCoords = (Input.ShadowMapCoords[1].xyz / Input.ShadowMapCoords[1].w);
			float shadow0 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[1], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[1], shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
			float shadow1 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[2], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[2], shadowMapCoords);

			shadowAmount = mix(shadow0, shadow1, c1);
		}
		else if (c2 > 0.0 && c2 < 1.0)
		{
			// Sample 2 & 3
			vec3 shadowMapCoords = (Input.ShadowMapCoords[2].xyz / Input.ShadowMapCoords[2].w);
			float shadow0 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[2], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[2], shadowMapCoords);
			shadowMapCoords = (Input.ShadowMapCoords[3].xyz / Input.ShadowMapCoords[3].w);
			float shadow1 = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[3], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[3], shadowMapCoords);

			shadowAmount = mix(shadow0, shadow1, c2);
		}
		else
		{
			vec3 shadowMapCoords = (Input.ShadowMapCoords[CascadeIndex].xyz / Input.ShadowMapCoords[CascadeIndex].w);
			shadowAmount = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[CascadeIndex], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[CascadeIndex], shadowMapCoords);
		}
	}
	else
	{
		vec3 shadowMapCoords = (Input.ShadowMapCoords[CascadeIndex].xyz / Input.ShadowMapCoords[CascadeIndex].w);
		shadowAmount = u_SoftShadows ? PCSS_DirectionalLight(u_ShadowMapTexture[CascadeIndex], shadowMapCoords, u_LightSize) : HardShadows_DirectionalLight(u_ShadowMapTexture[CascadeIndex], shadowMapCoords);
	}

	float NdotL = dot(m_Params.Normal, u_DirectionalLights.Direction);
	NdotL = smoothstep(0.0, 0.4, NdotL + 0.2);
	shadowAmount *= (NdotL * 1.0);

	vec3 iblContribution = IBL(F0, Lr) * u_EnvironmentMapIntensity;
	vec3 lightContribution = u_DirectionalLights.Multiplier > 0.0f ? (Lighting(F0) * shadowAmount) : vec3(0.0f);

	o_Color = vec4(lightContribution + iblContribution, 1.0);

	// Bloom
	float brightness = dot(o_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	o_BloomColor = vec4(0.0, 0.0, 0.0, 1.0);

	if (brightness > u_BloomThreshold) // TODO: make u_BloomThreshold a thing
		o_BloomColor = o_Color;

	if (u_ShowCascades)
	{
		switch (CascadeIndex)
		{
			case 0:
				o_Color.rgb *= vec3(1.0f, 0.25f, 0.25f);
				break;

			case 1:
				o_Color.rgb *= vec3(0.25f, 1.0f, 0.25f);
				break;

			case 2:
				o_Color.rgb *= vec3(0.25f, 0.25f, 1.0f);
				break;

			case 3:
				o_Color.rgb *= vec3(1.0f, 1.0f, 0.25f);
				break;
		}
	}
}

