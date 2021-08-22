#shader vertex

#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in int in_EntityID;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
};

out vec4 v_Color;
out flat int v_EntityID;

void main()
{
	v_Color = in_Color;
	v_EntityID = in_EntityID;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0f);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ObjectID;

in vec4 v_Color;
in flat int v_EntityID;

void main()
{
	Color = v_Color;
	ObjectID = v_EntityID;
}

