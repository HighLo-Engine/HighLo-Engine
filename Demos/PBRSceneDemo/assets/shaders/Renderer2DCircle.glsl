#shader vertex

#version 450 core

layout(location = 0) in vec3 in_WorldPosition;
layout(location = 1) in float in_Thickness;
layout(location = 2) in vec2 in_LocalPosition;
layout(location = 3) in vec4 in_Color;
layout(location = 4) in int in_EntityID;

layout(std140, binding = 3) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 LocalPosition;
	float Thickness;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.LocalPosition = in_LocalPosition;
	Output.Thickness = in_Thickness;
	Output.Color = in_Color;
	v_EntityID = in_EntityID;
	gl_Position = u_ViewProjection * vec4(in_WorldPosition, 1.0f);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ObjectID;

struct VertexOutput
{
	vec4 Color;
	vec2 LocalPosition;
	float Thickness;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityID;

void main()
{
	float fade = 0.01f;
	float distance = sqrt(dot(Input.LocalPosition, Input.LocalPosition));
	if (distance > 1.0f || distance < 1.0f - Input.Thickness - fade)
		discard;

	float alpha = 1.0f - smoothstep(1.0f - fade, 1.0f, distance);
	alpha *= smoothstep(1.0f - Input.Thickness - fade, 1.0f - Input.Thickness, distance);

	Color = Input.Color;
	Color.a = alpha;
	ObjectID = v_EntityID;
}
