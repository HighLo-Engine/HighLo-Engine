#shader vertex
#version 450 core

layout(location = 0) in vec3 in_Position;

layout (std140, binding = 0) uniform VS_SceneBuffer
{
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
	vec3 u_CameraPosition;
	float u_Padding01;
    vec3 u_LightPosition;
    float u_Padding02;
    vec3 u_LightColor;
    float u_Padding03;
};

out vec3 pass_TextureCoords;

void main()
{
	pass_TextureCoords = in_Position;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(in_Position, 1.0);
} 

#shader pixel
#version 450 core

in vec3 pass_TextureCoords;

out vec4 Color;
layout(binding = 0) uniform samplerCube SkyTextureSampler;

void main()
{
	Color = texture(SkyTextureSampler, -pass_TextureCoords);
}
