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

layout(location = 5) in ivec4 a_BoneIndices;
layout(location = 6) in vec4 a_BoneWeights;

void main()
{

}

#version 450 core
#pragma shader:fragment

#include <Buffers.glslh>
#include <PBR.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

void main()
{

}

