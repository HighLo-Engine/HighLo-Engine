#version 450 core
#pragma shader:vertex

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in float in_TexIndex;
layout(location = 4) in float in_TilingFactor;
layout(location = 5) in int in_EntityID;

layout(location = 6) in vec4 a_MRow0;
layout(location = 7) in vec4 a_MRow1;
layout(location = 8) in vec4 a_MRow2;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

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
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);

	Output.Color = in_Color;
	Output.TexCoord = in_TexCoord;
	Output.TexIndex = in_TexIndex;
	Output.TilingFactor = in_TilingFactor;
	v_EntityID = in_EntityID;
	gl_Position = u_ViewProjection * transform * vec4(in_Position, 1.0f);
}

#version 450 core
#pragma shader:fragment

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

