#shader vertex
#version 450 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec3 in_Normal;
layout (location = 3) in vec3 in_Tangent;
layout (location = 4) in vec3 in_Binormal;
layout (location = 5) in ivec4 in_BoneIDs;
layout (location = 6) in vec4 in_BoneWeights;

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

layout (std140, binding = 1) uniform VS_ObjectBuffer
{
	mat4 u_Transform;
};

layout (std140, binding = 4) uniform BoneTransformsBuffer
{
	mat4 u_BoneTransforms[150];
};

out VS_TO_PS
{
	vec2 UV;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
    vec3 LightPosition;
    vec3 LightColor;
} VS_Output;

void main()
{
	mat4 BoneTransform = u_BoneTransforms[in_BoneIDs[0]] * in_BoneWeights[0];
    BoneTransform += u_BoneTransforms[in_BoneIDs[1]] * in_BoneWeights[1];
    BoneTransform += u_BoneTransforms[in_BoneIDs[2]] * in_BoneWeights[2];
    BoneTransform += u_BoneTransforms[in_BoneIDs[3]] * in_BoneWeights[3];

	vec4 LocalVertexPosition = BoneTransform * vec4(in_Position, 1.0);

    vec4 WorldPosition = u_Transform * LocalVertexPosition;
	vec4 ModelViewPosition = u_ViewMatrix * WorldPosition;
	vec4 FinalPosition = u_ProjectionMatrix * ModelViewPosition;

	VS_Output.UV = in_UV;
	VS_Output.Normal = mat3(u_Transform) * mat3(BoneTransform) * in_Normal;
	VS_Output.WorldPosition = vec3(WorldPosition);
	VS_Output.CameraPosition = u_CameraPosition;
    VS_Output.LightPosition = u_LightPosition;
    VS_Output.LightColor = u_LightColor;

	gl_Position = FinalPosition;
}

#shader pixel
#version 450 core

in VS_TO_PS
{
	vec2 UV;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
    vec3 LightPosition;
    vec3 LightColor;
} PS_Input;

out vec4 out_Color;

layout (std140, binding = 2) uniform MaterialDataBuffer
{
	vec4	u_Color;
	float	u_Roughness;
    float   u_Metallic;
};

layout(binding = 0) uniform sampler2D AlbedoTextureSampler;

vec4 CalculateAlbedoColor()
{
	return u_Color * texture(AlbedoTextureSampler, PS_Input.UV);
}

void main()
{
    out_Color = CalculateAlbedoColor();
} 
