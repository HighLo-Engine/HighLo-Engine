#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;

void main()
{
	Output.TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Texture;

//#ifdef __VULKAN__
//layout(push_constant) uniform Uniforms
//{
//	float Exposure;
//} u_Uniforms;
//#else
//layout(std140, binding = 15) uniform Uniforms
//{
//	float Exposure;
//} u_Uniforms;
//#endif

void main()
{
//	vec4 color = texture(u_Texture, Input.TexCoord);
//	color *= u_Uniforms.Exposure;
//	o_Color = color;
	o_Color = texture(u_Texture, Input.TexCoord);
}

