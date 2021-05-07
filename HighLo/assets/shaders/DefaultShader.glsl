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

	gl_Position = FinalPosition;
}

#shader pixel
#version 450 core

const float PI = 3.141592;
const float Epsilon = 0.00001;

const int LightCount = 1;

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

const float ao = 1.0;

struct Light {
	vec3 Direction;
	vec3 Radiance;
	float Multiplier;
};

in VS_TO_PS
{
	vec2 UV;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
} PS_Input;

out vec4 out_Color;

layout(binding = 0) uniform sampler2D DiffuseTextureSampler;

layout (std140, binding = 2) uniform MaterialDataBuffer
{
	vec4	u_Color;
	float	u_Roughness;
    float   u_Metallic;
};

vec4 CalculateTextureColor()
{
	return u_Color * texture(DiffuseTextureSampler, PS_Input.UV);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

const vec3 LightPosition = vec3(0.0f,  8.0f, -10.0f);
const vec3 LightColor = vec3(700.0f, 700.0f, 700.0f);

void main()
{
    vec4 TextureColor = CalculateTextureColor();

	vec3 N = normalize(PS_Input.Normal);
    vec3 V = normalize(PS_Input.CameraPosition - PS_Input.WorldPosition);

	vec3 F0 = mix(Fdielectric, vec3(TextureColor), u_Metallic);

    vec3 Lo = vec3(0.0);

	 // calculate light radiance
	vec3 L = normalize(LightPosition - PS_Input.WorldPosition);
    vec3 H = normalize(V + L);
    float distance = length(LightPosition - PS_Input.WorldPosition);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = LightColor * attenuation;

	// Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, u_Roughness);   
    float G   = GeometrySmith(N, V, L, u_Roughness);      
    vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, Epsilon);

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - u_Metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo += (kD * vec3(TextureColor) / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    vec3 ambient = vec3(0.03) * vec3(TextureColor) * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2)); 

    out_Color = vec4(color, TextureColor.a);
} 
