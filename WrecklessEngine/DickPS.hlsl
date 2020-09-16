struct VS_OUT
{
	float3 PosW : POSITION;
	float4 PosH : SV_POSITION;
};

float4 main(VS_OUT pin) : SV_TARGET
{
	return float4(pin.PosW, 1.0f);
}