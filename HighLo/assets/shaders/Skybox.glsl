#version 430 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
	mat4 u_InverseViewProjection;
};

/*
layout(push_constant) uniform Transform
{
	mat4 Transform;
} u_Renderer;
*/

layout(location = 0) out vec3 v_Position;

void main()
{
	vec4 pos = vec4(a_Position.xy, 1.0, 1.0);
	gl_Position = pos;

	v_Position = (u_InverseViewProjection * pos).xyz;
}

#version 430 core
#pragma shader:fragment

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Unused1;
layout(location = 2) out vec4 out_Unused2;

layout(binding = 1) uniform samplerCube u_Texture;
layout(location = 0) in vec3 v_Position;

layout(push_constant) uniform Uniforms
{
	float TextureLod;
	float Intensity;
} u_Uniforms;

void main()
{
	out_Unused1 = vec4(0.0);
	out_Unused2 = vec4(0.0);

	out_Color = textureLod(u_Texture, v_Position, u_Uniforms.TextureLod) * u_Uniforms.Intensity;
}

