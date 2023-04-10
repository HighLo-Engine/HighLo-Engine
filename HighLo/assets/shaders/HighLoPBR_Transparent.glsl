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
	//layout(binding = 5) uniform sampler2D u_DiffuseTexture;
	//layout(binding = 6) uniform sampler2D u_NormalTexture;
	//layout(binding = 7) uniform sampler2D u_MetalnessTexture;
	//layout(binding = 8) uniform sampler2D u_RoughnessTexture;

	// Environment maps
	//layout(binding = 9) uniform samplerCube u_EnvRadianceTex;
	//layout(binding = 10) uniform samplerCube u_EnvIrradianceTex;

	// BRDF LUT
	//layout(binding = 11) uniform sampler2D u_BRDFLUTTexture;

	// TODO
	// Shadow maps
	//layout(binding = 12) uniform sampler2DArray u_ShadowMapTexture;
	//layout(binding = 21) uniform sampler2D u_SpotShadowTexture;
#endif

void main()
{
	// TEMP
	o_Color = vec4(u_MaterialUniforms.DiffuseColor, 1.0);
}

