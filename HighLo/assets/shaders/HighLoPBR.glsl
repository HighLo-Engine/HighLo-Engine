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

layout(location = 5) in vec4 a_MRow0;
layout(location = 6) in vec4 a_MRow1;
layout(location = 7) in vec4 a_MRow2;

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
//	mat4 transform = mat4(
//		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
//		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
//		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
//		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
//	);

	mat4 transform = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
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

#ifdef __VULKAN__
	layout(push_constant) uniform Material
	{
		vec3 DiffuseColor;
		float Roughness;
		float Metalness;
		float Emission;
		float Transparency;
		float EnvMapRotation;
		int UseNormalMap;
		int Padding1;
	} u_MaterialUniforms;
#else
	layout(std140, binding = 13) uniform Material
	{
		vec3 DiffuseColor;
		float Metalness;
		float Roughness;
		float Emission;
		float Transparency;
		float EnvMapRotation;
		int UseNormalMap;
		int Padding1;
	} u_MaterialUniforms;
#endif

layout(location = 0) in VertexOutput Input;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_ViewNormalsLuminance;
layout(location = 2) out vec4 o_MetalnessRoughness;

#ifdef __VULKAN__
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
	layout(binding = 5) uniform sampler2D u_DiffuseTexture;
	layout(binding = 6) uniform sampler2D u_NormalTexture;
	layout(binding = 7) uniform sampler2D u_MetalnessTexture;
	layout(binding = 8) uniform sampler2D u_RoughnessTexture;

	// Environment maps
	layout(binding = 9) uniform samplerCube u_EnvRadianceTex;
	layout(binding = 10) uniform samplerCube u_EnvIrradianceTex;

	// BRDF LUT
	layout(binding = 11) uniform sampler2D u_BRDFLUTTexture;

	// Shadow maps
	layout(binding = 12) uniform sampler2DArray u_ShadowMapTexture;
	layout(binding = 21) uniform sampler2D u_SpotShadowTexture;
#endif

void main()
{
	// Standard PBR inputs
	vec4 diffuseTexColor = texture(u_DiffuseTexture, Input.TexCoord);
	m_Params.Diffuse = diffuseTexColor.rgb * u_MaterialUniforms.DiffuseColor;
	float alpha = diffuseTexColor.a;

	m_Params.Metalness = texture(u_MetalnessTexture, Input.TexCoord).r * u_MaterialUniforms.Metalness;
	m_Params.Roughness = texture(u_RoughnessTexture, Input.TexCoord).r * u_MaterialUniforms.Roughness;
	o_MetalnessRoughness = vec4(m_Params.Metalness, m_Params.Roughness, 0.f, 1.f);
	m_Params.Roughness = max(m_Params.Roughness, 0.05); // Minimum roughness of 0.05 to keep specular highlight

	// Normals (either from vertex or map)
	m_Params.Normal = normalize(Input.Normal);
	if (u_MaterialUniforms.UseNormalMap == 1)
	{
		m_Params.Normal = normalize(texture(u_NormalTexture, Input.TexCoord).rgb * 2.0f - 1.0f);
		m_Params.Normal = normalize(Input.WorldNormals * m_Params.Normal);
	}
	// View normals
	o_ViewNormalsLuminance.xyz = Input.CameraView * m_Params.Normal;

	m_Params.View = normalize(u_Scene.CameraPosition - Input.WorldPosition);
	m_Params.NdotV = max(dot(m_Params.Normal, m_Params.View), 0.0);

	// Specular reflection vector
	vec3 Lr = 2.0 * m_Params.NdotV * m_Params.Normal - m_Params.View;

	// Fresnel reflectance, metals use diffuse
	vec3 F0 = mix(Fdielectric, m_Params.Diffuse, m_Params.Metalness);

	// Direct lighting
	//vec3 lightContribution = CalculateDirLights(F0);
	//lightContribution += CalculatePointLights(F0, Input.WorldPosition);
	//lightContribution += m_Params.Diffuse * u_MaterialUniforms.Emission;
	vec3 lightContribution = m_Params.Diffuse * u_MaterialUniforms.Emission;

	// Indirect lighting
	vec3 iblContribution = IBL(F0, Lr, u_EnvRadianceTex, u_EnvIrradianceTex, u_BRDFLUTTexture, u_MaterialUniforms.EnvMapRotation, m_Params.NdotV, m_Params.Roughness, m_Params.Metalness, m_Params.Diffuse, m_Params.Normal, m_Params.View);
	iblContribution *= u_Scene.EnvironmentMapIntensity;

	o_Color = vec4(iblContribution + lightContribution, 1.0);
}

