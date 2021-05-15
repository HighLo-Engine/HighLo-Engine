#shader vertex

cbuffer VS_SceneBuffer : register(b0)
{
	float4x4 u_ProjectionMatrix;
	float4x4 u_ViewMatrix;
	float3   u_CameraPosition;
	float    u_Padding01;
};

cbuffer VS_ObjectBuffer : register(b1)
{
	float4x4 u_Transform;
};

struct VS_INPUT
{
	float3 in_Position : POSITION;
	float2 in_UV : UV;
	float3 in_Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 out_Position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 WorldPosition = mul(u_Transform, float4(input.in_Position, 1.0));
	float4 ModelViewPosition = mul(u_ViewMatrix, WorldPosition);
	float4 FinalPosition = mul(u_ProjectionMatrix, ModelViewPosition);

	output.out_Position = FinalPosition;

	return output;
}

#shader pixel

struct PS_INPUT
{
	float4 in_Position : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 FinalColor = float4(0.5, 0.2, 0.8, 1.0);
	return FinalColor;
}