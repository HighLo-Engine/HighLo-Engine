#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat float v_TexIndex;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_Camera.ViewProjectionMatrix * vec4(a_Position, 1.0);
}

#version 450 core
#pragma shader:fragment

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out vec4 o_Color;

layout(location = 0) in VertexOutput Input;
layout(location = 5) in flat float v_TexIndex;
layout(binding = 1) uniform sampler2D u_FontAtlases[32];

void main()
{
	o_Color = texture(u_FontAtlases[int(v_TexIndex)], Input.TexCoord) * Input.Color;
}

