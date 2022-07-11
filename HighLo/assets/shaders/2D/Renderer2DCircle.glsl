#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in float a_Thickness;
layout(location = 2) in vec2 a_LocalPosition;
layout(location = 3) in vec4 a_Color;
layout(location = 4) in int a_EntityID;

layout(location = 5) in vec4 a_MRow0;
layout(location = 6) in vec4 a_MRow1;
layout(location = 7) in vec4 a_MRow2;

struct VertexOutput
{
	vec4 Color;
	vec2 LocalPosition;
	float Thickness;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat int v_EntityID;

void main()
{
	/*
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);
	*/

	Output.LocalPosition = a_LocalPosition;
	Output.Thickness = a_Thickness;
	Output.Color = a_Color;
	v_EntityID = a_EntityID;
	//gl_Position = u_Camera.ViewProjectionMatrix * transform * vec4(a_WorldPosition, 1.0f);
	gl_Position = u_Camera.ViewProjectionMatrix * vec4(a_WorldPosition, 1.0f);
}


#version 450 core
#pragma shader:fragment

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_ObjectID;

struct VertexOutput
{
	vec4 Color;
	vec2 LocalPosition;
	float Thickness;
};

layout(location = 0) in VertexOutput Input;
layout(location = 5) in flat int v_EntityID;

void main()
{
	float fade = 0.01f;
	float distance = sqrt(dot(Input.LocalPosition, Input.LocalPosition));
	if (distance > 1.0f || distance < 1.0f - Input.Thickness - fade)
		discard;

	float alpha = 1.0f - smoothstep(1.0f - fade, 1.0f, distance);
	alpha *= smoothstep(1.0f - Input.Thickness - fade, 1.0f - Input.Thickness, distance);

	o_Color = Input.Color;
	o_Color.a = alpha;
	o_ObjectID = v_EntityID;
}
