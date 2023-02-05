#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

#ifdef __VULKAN__
layout (push_constant) uniform Transform
{
	uint Cascade;
} u_Renderer;
#else
layout(binding = 0, std140) uniform Transform
{
	uint Cascade;
} u_Renderer;
#endif

void main()
{
	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
	);

	gl_Position = u_DirShadow.DirLightMatrices[u_Renderer.Cascade] * transform * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

void main()
{
}

