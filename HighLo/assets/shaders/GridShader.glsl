#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Row1;
layout(location = 2) in vec4 a_Row2;
layout(location = 3) in vec4 a_Row3;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

void main()
{
	mat4 transform = mat4(
		vec4(a_Row1.x, a_Row2.x, a_Row3.x, 0.0),
		vec4(a_Row1.y, a_Row2.y, a_Row3.y, 0.0),
		vec4(a_Row1.z, a_Row2.z, a_Row3.z, 0.0),
		vec4(a_Row1.w, a_Row2.w, a_Row3.w, 1.0));

	gl_Position = u_ViewProjection * transform * vec4(a_Position, 1.0);
}

#shader pixel
#version 450 core

layout(location = 0) out vec4 out_Color;

layout(push_constant) uniform Material
{
	layout(offset = 64) vec4 Color;
} u_MaterialUniforms;

void main()
{
	out_Color = u_MaterialUniforms.Color;
}

