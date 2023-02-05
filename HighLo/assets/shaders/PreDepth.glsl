#version 450 core
#pragma shader:vertex

#include <Buffers.glslh>

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Binormal;

//layout(location = 0) out float v_LinearDepth;

precise invariant gl_Position;

void main()
{
	mat4 transform = mat4(
		vec4(u_ObjectTransformation.Row0.x, u_ObjectTransformation.Row1.x, u_ObjectTransformation.Row2.x, 0.0),
		vec4(u_ObjectTransformation.Row0.y, u_ObjectTransformation.Row1.y, u_ObjectTransformation.Row2.y, 0.0),
		vec4(u_ObjectTransformation.Row0.z, u_ObjectTransformation.Row1.z, u_ObjectTransformation.Row2.z, 0.0),
		vec4(u_ObjectTransformation.Row0.w, u_ObjectTransformation.Row1.w, u_ObjectTransformation.Row2.w, 1.0)
	);

	vec4 worldPos = transform * vec4(a_Position, 1.0);
	//v_LinearDepth = -(u_ViewMatrix * worldPos).z;

	gl_Position = u_Camera.ViewProjectionMatrix * worldPos;
}

#version 450 core
#pragma shader:fragment

//layout(location = 0) out vec4 o_LinearDepth;
//layout(location = 0) in float v_LinearDepth;

void main()
{
	//o_LinearDepth = vec4(v_LinearDepth, 0.0, 0.0, 1.0);
}

