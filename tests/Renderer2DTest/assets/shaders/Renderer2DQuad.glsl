#shader vertex
#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in float in_TexIndex;
layout(location = 4) in float in_TilingFactor;

layout(std140, binding = 3) uniform Camera
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

void main()
{
	Output.Color = in_Color;
	Output.TexCoord = in_TexCoord;
	Output.TexIndex = in_TexIndex;
	Output.TilingFactor = in_TilingFactor;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0f);
}

#shader pixel
#version 450 core

layout(location = 0) out vec4 Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(binding = 1) uniform sampler2D u_Textures[32];

void main()
{
	//Color = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color;
	//Color = vec4(1.0f, 0.2f, 0.3f, 1.0f);
	Color = vec4(Input.TexCoord, 0.0f, 0.0f);
}

