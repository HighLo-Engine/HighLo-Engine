#version 450 core
#pragma shader:vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;

layout(location = 4) in vec4 a_MRow0;
layout(location = 5) in vec4 a_MRow1;
layout(location = 6) in vec4 a_MRow2;

layout(std140, binding = 0) uniform Camera
{
	mat4 ViewProjection;
} u_Camera;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat float v_TexIndex;

void main()
{
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);

	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_Camera.ViewProjection * transform * vec4(a_Position, 1.0);
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

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float ScreenPixelRange()
{
	float pxRange = 2.0f;
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlases[int(v_TexIndex)], 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(Input.TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
	vec4 bgColor = vec4(Input.Color.rgb, 0.0);
	vec4 fgColor = Input.Color;

	vec3 msd = texture(u_FontAtlases[int(v_TexIndex)], Input.TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = ScreenPixelRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	o_Color = mix(bgColor, fgColor, opacity);
	if (opacity == 0.0)
		discard;
}

