struct VertexOutput
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BITANGENT;
	float2 TexCoords : TEXCOORD;
};

Texture2D g_Albedo : register(t0);

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};


float4 main(VertexOutput pin) : SV_TARGET
{
	return g_Albedo.Sample(samLinear, pin.TexCoords);
}