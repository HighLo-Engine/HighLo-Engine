#shader vertex
#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in float in_TexIndex;
layout(location = 4) in float in_TilingFactor;
layout(location = 5) in int in_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(push_constant) uniform Transform
{
	mat4 Transform;
} u_Renderer;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat int v_EntityID;

void main()
{
	Output.Color = in_Color;
	Output.TexCoord = in_TexCoord;
	Output.TexIndex = in_TexIndex;
	Output.TilingFactor = in_TilingFactor;
	gl_Position = u_ViewProjection * u_Renderer.Transform * vec4(in_Position, 1.0f);

	v_EntityID = in_EntityID;
}

#shader pixel
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_ObjectID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 5) in flat int v_EntityID;

layout(binding = 1) uniform sampler2D u_Textures[32];

void main()
{
	o_Color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color;
	o_ObjectID = v_EntityID;
}

