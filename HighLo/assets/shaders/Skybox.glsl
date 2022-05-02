#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec3 v_Position;

void main()
{
	vec4 pos = vec4(a_Position.xy, 1.0, 1.0);
	gl_Position = pos;

	v_Position = (u_Camera.InverseViewProjectionMatrix * pos).xyz;
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_Unused1;
layout(location = 2) out vec4 o_Unused2;

layout(binding = 1) uniform samplerCube u_Texture;

#ifdef __VULKAN__
layout(push_constant) uniform Uniforms
{
	float TextureLod;
	float Intensity;
} u_Uniforms;
#else
layout(binding = 0, std140) uniform Uniforms
{
	float TextureLod;
	float Intensity;
} u_Uniforms;
#endif

layout(location = 0) in vec3 v_Position;

void main()
{
	o_Color = textureLod(u_Texture, v_Position, u_Uniforms.TextureLod) * u_Uniforms.Intensity;
	o_Unused1 = vec4(0.0);
	o_Unused2 = vec4(0.0);
}

