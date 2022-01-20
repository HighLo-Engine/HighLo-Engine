#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};

layout (push_constant) uniform Transform
{
	mat4 Transform;
} u_Renderer;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMatrix * u_Renderer.Transform * vec4(a_Position, 1.0);
}

#shader pixel
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out vec4 o_unused0;
layout(location = 2) out vec4 o_unused1;

layout(location = 0) in vec2 v_TexCoord;

layout(push_constant) uniform Settings
{
	layout(offset = 64) float Scale;
	float Size;
} u_Settings;

float grid(vec2 st, float res)
{
	vec2 grid = fract(st);
	return step(res, grid.x) * step(res, grid.y);
}

void main()
{
	float x = grid(v_TexCoord * u_Settings.Scale, u_Settings.Size);
	o_Color = vec4(vec3(0.2), 0.5) * (1.0 - x);
	o_unused0 = vec4(0.0);
	o_unused1 = vec4(0.0);

	if (o_Color.a == 0.0)
		discard;
}

