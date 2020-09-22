TextureCube g_CubeMap : register(t0);
SamplerState g_SamplerState : register(s0);

struct VS_OUT
{
	float3 PosL : POSITION;
	float4 PosH : SV_POSITION;
};


float4 main(VS_OUT pin) : SV_TARGET
{
	//return float4(0, 1, 1, 1);
	return g_CubeMap.Sample(g_SamplerState, pin.PosL);
}