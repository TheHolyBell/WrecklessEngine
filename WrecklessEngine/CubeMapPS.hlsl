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
	//return float4(0, 1, 1, 1);
	//return g_CubeMap.Sample(g_SamplerState, pin.PosL);

	float red = -0.00022 * (abs(pin.PosW.y) - 2800) + 0.18;
	float green = -0.00025 * (abs(pin.PosW.y) - 2800) + 0.27;
	float blue = -0.00019 * (abs(pin.PosW.y) - 2800) + 0.47;

	return float4(red, green, blue, 1.0f);
}