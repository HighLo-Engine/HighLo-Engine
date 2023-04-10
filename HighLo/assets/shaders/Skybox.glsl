#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct VertexOutput
{
	vec3 Position;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;

void main()
{
	vec4 pos = vec4(a_Position.xy, 0.0, 1.0);
	Output.Position = mat3(u_Camera.InverseViewMatrix) * vec3(u_Camera.InverseProjectionMatrix * pos);
	Output.TexCoord = a_TexCoord;
	gl_Position = pos;
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec3 Position;
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

#ifdef __VULKAN__
	layout(push_constant) uniform SkyboxUniform
	{
		float TextureLod;
		float Intensity;
	} u_SkyboxUniform;
#else
	layout(std140, binding = 31) uniform SkyboxUniform
	{
		float TextureLod;
		float Intensity;
	} u_SkyboxUniform;
#endif

layout(binding = 1) uniform samplerCube u_Texture;

void main()
{
	o_Color = textureLod(u_Texture, Input.Position, u_SkyboxUniform.TextureLod) * u_SkyboxUniform.Intensity;
	o_Color.a = 1.0;
}

