#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

#define MAX_BONES 100

layout(location = 0) in vec3 a_Position;
layout(location = 1) in ivec4 a_BoneIndices;
layout(location = 2) in vec4 a_BoneWeights;

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

	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
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

