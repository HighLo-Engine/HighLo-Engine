#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

#define MAX_BONES 100

layout(location = 0) in vec3 a_Position;
layout(location = 1) in ivec4 a_BoneIndices;
layout(location = 2) in vec4 a_BoneWeights;

layout(location = 3) in vec4 a_MRow0;
layout(location = 4) in vec4 a_MRow1;
layout(location = 5) in vec4 a_MRow2;

#ifdef __VULKAN__
layout(push_constant) uniform Uniforms
{
	mat4 BoneTransforms[MAX_BONES];
} u_Uniforms;
#else
layout(binding = 0, std140) uniform Uniforms
{
	mat4 BoneTransforms[MAX_BONES];
} u_Uniforms;
#endif

void main()
{
	mat4 boneTransform = u_Uniforms.BoneTransforms[a_BoneIndices[0]] * a_BoneWeights[0];
    boneTransform += u_Uniforms.BoneTransforms[a_BoneIndices[1]] * a_BoneWeights[1];
    boneTransform += u_Uniforms.BoneTransforms[a_BoneIndices[2]] * a_BoneWeights[2];
    boneTransform += u_Uniforms.BoneTransforms[a_BoneIndices[3]] * a_BoneWeights[3];

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

	vec4 localPosition = boneTransform * vec4(a_Position, 1.0);
	gl_Position = u_Camera.ViewProjectionMatrix * transform * localPosition;
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0);
}

