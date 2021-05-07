#shader vertex

#version 450 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UV;

layout (std140, binding = 0) uniform VS_SceneBuffer
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
	vec3 u_CameraPosition;
	float u_Padding01;
};

layout (std140, binding = 1) uniform VS_ObjectBuffer
{
	mat4 u_Transform;
};

out VS_TO_PS
{
	vec2 UV;
} VS_Output;

void main()
{
    vec4 WorldPosition = u_Transform * vec4(in_Position, 1.0);
	vec4 ModelViewPosition = u_ViewMatrix * WorldPosition;
	vec4 FinalPosition = u_ProjectionMatrix * ModelViewPosition;

	VS_Output.UV = in_UV;

	gl_Position = FinalPosition;
}

#shader pixel

#version 450 core

in VS_TO_PS
{
	vec2 UV;
} PS_Input;

out vec4 out_Color;

layout(binding = 0) uniform sampler2D DiffuseTextureSampler;

layout (std140, binding = 2) uniform MaterialDataBuffer
{
	vec4	u_Color;
	float	u_Roughness;
};

vec4 CalculateTextureColor()
{
	return u_Color * texture(DiffuseTextureSampler, PS_Input.UV);
}

void main()
{
	vec4 TextureColor = CalculateTextureColor();

    out_Color = TextureColor;
} 
