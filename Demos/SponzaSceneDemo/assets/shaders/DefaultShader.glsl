#shader vertex
#version 450 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec3 in_Normal;

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
    vec4 WorldPosition = u_Transform * vec4(in_Position, 1.0);
	vec4 ModelViewPosition = u_ViewMatrix * WorldPosition;
	vec4 FinalPosition = u_ProjectionMatrix * ModelViewPosition;

	VS_Output.UV = in_UV;
	VS_Output.Normal = mat3(u_Transform) * in_Normal;
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

layout(binding = 0) uniform sampler2D DiffuseTextureSampler;
layout(binding = 1) uniform sampler2D NormalTextureSampler;

layout (std140, binding = 2) uniform MaterialDataBuffer
{
	vec4	u_Color;
	float	u_Roughness;
    float   u_Metallic;
    float   u_UnkPadding0;
    float   u_UnkPadding1;
	vec3    u_SpecularHighlight;
	float   u_Shininess;
};

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(NormalTextureSampler, PS_Input.UV).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(PS_Input.WorldPosition);
    vec3 Q2  = dFdy(PS_Input.WorldPosition);
    vec2 st1 = dFdx(PS_Input.UV);
    vec2 st2 = dFdy(PS_Input.UV);

    vec3 N   = normalize(PS_Input.Normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec4 CalculateDiffuseColor()
{
	return u_Color * texture(DiffuseTextureSampler, PS_Input.UV);
}

void main()
{
    vec4 diffuseColor = CalculateDiffuseColor();
	vec3 normal = normalize(getNormalFromMap());

	// Ambient
    vec3 ambient = normalize(PS_Input.LightColor) * vec3(diffuseColor) * u_Shininess;
  	
    // Diffuse
    vec3 lightDir = normalize(PS_Input.LightPosition - PS_Input.WorldPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = normalize(PS_Input.LightColor) * (diff * vec3(diffuseColor));
    
    // Specular
    vec3 viewDir = normalize(PS_Input.CameraPosition - PS_Input.WorldPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
    vec3 specular = normalize(PS_Input.LightColor) * (spec * u_SpecularHighlight) * u_Shininess;  
        
    vec3 result = ambient + diffuse + specular;

    out_Color = vec4(result, diffuseColor.a);
} 
