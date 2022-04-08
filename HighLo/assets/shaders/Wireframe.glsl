#version 450 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BiNormal;

layout(location = 5) in vec4 a_MRow0;
layout(location = 6) in vec4 a_MRow1;
layout(location = 7) in vec4 a_MRow2;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

/*
layout(push_constant) uniform Transform
{
	mat4 Transform;
} u_Renderer;
*/

void main()
{
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);

	gl_Position = u_ViewProjection * transform * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;

layout(push_constant) uniform MaterialUniforms
{
	layout(offset = 64) vec4 Color;
} u_MaterialUniforms;

void main()
{
	o_Color = u_MaterialUniforms.Color;
}

