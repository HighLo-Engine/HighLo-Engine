#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

layout(location = 5) in ivec4 a_BoneIndices;
layout(location = 6) in vec4 a_BoneWeights;

// Transform buffer
layout(location = 7) in vec4 a_MRow0;
layout(location = 8) in vec4 a_MRow1;
layout(location = 9) in vec4 a_MRow2;

#ifdef __VULKAN__
layout(push_constant) uniform Transform
{
	mat4 u_BoneTransform[100];
} u_Transform;
#else
layout(binding = 22, std140) uniform Transform
{
	mat4 u_BoneTransform[100];
} u_Transform;
#endif

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

	mat4 objectTransform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);

	gl_Position = u_ViewProjectionMatrix * objectTransform * localPosition;
}

#version 450 core
#pragma shader:fragment

#include <Buffers.glslh>
#include <PBR.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

layout(location = 0) out vec4 o_Color;

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

void main()
{
	o_Color = vec4(1.0);
}

