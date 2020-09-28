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
SamplerState g_SamplerState : register(s0);


float4 main(VertexOutput pin) : SV_TARGET
{
	return g_Albedo.Sample(g_SamplerState, pin.TexCoords);
}