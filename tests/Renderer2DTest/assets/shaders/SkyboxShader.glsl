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

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(in_Position, 1.0);
	pass_TextureCoords = in_Position;
} 

#shader pixel
#version 450 core

in vec3 pass_TextureCoords;

layout(binding = 0) uniform samplerCube SkyTextureSampler;

out vec4 o_Color;

void main()
{
	o_Color = texture(SkyTextureSampler, -pass_TextureCoords);
}
