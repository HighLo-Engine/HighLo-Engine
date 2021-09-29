#shader vertex

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in vec4 a_Color;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 6) uniform Transform
{
	mat4 u_Transform;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat float TexIndex;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;
layout(location = 5) in flat float TexIndex;

layout(binding = 1) uniform sampler2D u_FontAtlases[32];

float Median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float ScreenPxRange()
{
	float pxRange = 2.0f;
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlases[int(TexIndex)], 0));
	vec2 screenTexSize = vec2(1.0f) / fwidth(Input.TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
	// TODO: Outlines
	vec4 bgColor = vec4(Input.Color.rgb, 0.0);
	vec4 fgColor = Input.Color;

	vec3 msd = texture(u_FontAtlases[int(TexIndex)], Input.TexCoord).rgb;
	float sd = Median(msd.r, msd.g, msd.b);
	float screenPxDistance = ScreenPxRange() * (sd - 0.5f);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	
	Color = mix(bgColor, fgColor, opacity);
	if (opacity == 0.0)
		discard;

}

