#version 450 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

layout (std140, binding = 1) uniform ShadowData
{
	mat4 u_ViewProjectionMatrix[4];
};

layout (push_constant) uniform Transform
{
	mat4 Transform;
	uint Cascade;
} u_Renderer;


void main()
{
	gl_Position = u_ViewProjectionMatrix[u_Renderer.Cascade] * u_Renderer.Transform * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

void main()
{
}

