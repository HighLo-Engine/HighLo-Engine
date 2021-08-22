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

out vec2 v_LocalPosition;
out float v_Thickness;
out vec4 v_Color;
out flat int v_EntityID;

void main()
{
	v_LocalPosition = in_LocalPosition;
	v_Thickness = in_Thickness;
	v_Color = in_Color;
	v_EntityID = in_EntityID;
	gl_Position = u_ViewProjection * vec4(in_WorldPosition, 1.0f);
}

#shader pixel

#version 450 core

layout(location = 0) out vec4 Color;
layout(location = 1) out int ObjectID;

in vec2 v_LocalPosition;
in float v_Thickness;
in vec4 v_Color;
in flat int v_EntityID;

void main()
{
	float fade = 0.01f;
	float distance = sqrt(dot(v_LocalPosition, v_LocalPosition));
	if (distance > 1.0f || distance < 1.0f - v_Thickness - fade)
		discard;

	float alpha = 1.0f - smoothstep(1.0f - fade, 1.0f, distance);
	alpha *= smoothstep(1.0f - v_Thickness - fade, 1.0f - v_Thickness, distance);

	Color = v_Color;
	Color.a = alpha;
	ObjectID = v_EntityID;
}
