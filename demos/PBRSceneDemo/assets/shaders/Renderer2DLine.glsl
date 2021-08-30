#shader vertex

#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in int in_EntityID;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = in_Color;
	v_EntityID = in_EntityID;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0f);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ObjectID;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityID;

void main()
{
	Color = Input.Color;
	ObjectID = v_EntityID;
}

