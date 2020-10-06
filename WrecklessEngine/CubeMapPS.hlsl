TextureCube g_CubeMap : register(t0);
SamplerState g_SamplerState : register(s0);

struct VS_OUT
{
	float3 PosL : POSITION;
	float3 PosW : POSITION1;
	float4 PosH : SV_POSITION;
};


float4 main(VS_OUT pin) : SV_TARGET
{
	return g_CubeMap.Sample(g_SamplerState, pin.PosL);
}