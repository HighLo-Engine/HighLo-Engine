#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_ObjectID;

layout (std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct Vertex
{
	vec2 TexCoord;
	vec4 Color;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) out Vertex Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TexIndex = a_TexIndex;
	Output.TilingFactor = a_TilingFactor;
	v_EntityID = a_ObjectID;
	
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ObjectID;

struct Vertex
{
	vec2 TexCoord;
	vec4 Color;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) in Vertex Input;
layout(location = 4) in flat int v_EntityID;
layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	Color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color;
	ObjectID = v_EntityID;
}

