#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in float a_Thickness;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec2 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness;

void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Thickness = a_Thickness;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 Color;

in vec2 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;

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
}