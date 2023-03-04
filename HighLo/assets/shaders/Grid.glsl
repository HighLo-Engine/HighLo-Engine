#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
	);

	v_TexCoord = a_TexCoord;
	gl_Position = u_Camera.ViewProjectionMatrix * transform * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec2 v_TexCoord;

#ifdef __VULKAN__
	layout(push_constant) uniform Settings
	{
		layout(offset = 64) float Scale;
		float Size;
	} u_Settings;
#else
	layout(std140, binding = 42) uniform Settings
	{
		layout(offset = 64) float Scale;
		float Size;
	} u_Settings;
#endif

float grid(vec2 st, float res)
{
	vec2 grid = fract(st);
	return step(res, grid.x) * step(res, grid.y);
}

void main()
{
	float x = grid(v_TexCoord * 16.025, 0.025);
	o_Color = vec4(vec3(0.2), 0.5) * (1.0 - x);

	if (o_Color.a == 0.0)
		discard;
}

