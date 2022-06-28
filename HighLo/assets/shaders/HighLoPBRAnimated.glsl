#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

void main()
{
	// TODO
	gl_Position = vec4(1.0);
}

#version 450 core
#pragma shader:fragment

#include <Buffers.glslh>
#include <PBR.glslh>
#include <Lighting.glslh>
#include <ShadowMapping.glslh>

layout(location = 0) out vec4 o_Color;

void main()
{
	o_Color = vec4(1.0);
}

