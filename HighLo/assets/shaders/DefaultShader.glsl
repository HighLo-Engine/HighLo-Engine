#shader vertex

#version 450 core
layout (location = 0) in vec3 POSITION;

layout (std140, binding = 0) uniform VS_SceneBuffer
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
	vec3 u_CameraPosition;
	float u_Padding01;
};

layout (std140, binding = 1) uniform VS_ObjectBuffer
{
	mat4 u_Transform;
};

void main()
{
    vec4 WorldPosition = u_Transform * vec4(POSITION, 1.0);
	vec4 ModelViewPosition = u_ViewMatrix * WorldPosition;
	vec4 FinalPosition = u_ProjectionMatrix * ModelViewPosition;

	gl_Position = FinalPosition;
}

#shader pixel

#version 450 core

out vec4 out_Color;

void main()
{
    out_Color = vec4(0.5, 0.2, 0.8, 1.0);
} 
