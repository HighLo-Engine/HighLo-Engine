#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
	mat4 u_ViewInverseViewProjection;
	mat4 u_Projection;
	mat4 u_ViewMatrix;
};

layout(location = 0) out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#shader pixel
#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out vec4 Padding0;
layout(location = 2) out vec4 Padding1;

layout(std140, binding = 5) uniform Settings
{
	layout(offset = 64) float u_Scale;
	float u_Size;
};

layout(location = 0) in vec2 v_TexCoord;

float grid(vec2 st, float resolution)
{
	vec2 grid = fract(st);
	return step(resolution, grid.x) * step(resolution, grid.y);
}

void main()
{
	Padding0 = vec4(0.0);
	Padding1 = vec4(0.0);

	float x = grid(v_TexCoord * u_Scale, u_Size);
	Color = vec4(vec3(0.2), 0.5) * (1.0 - x);

	// TODO: measure if this downs our performance -> not every fragment can be assumed to exist with this, so there will be skipped pixels
	// @see: https://stackoverflow.com/a/8509158/12873837
	if (Color.a == 0.0)
		discard;
}

