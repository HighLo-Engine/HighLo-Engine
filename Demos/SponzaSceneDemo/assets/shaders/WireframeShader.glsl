#shader vertex

#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 6) uniform WireframeTransform
{
	mat4 u_Transform;
};

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;

layout(std140, binding = 8) uniform Material
{
	layout(offset = 64) vec4 u_Color;
};

void main()
{
	Color = u_Color;
}

