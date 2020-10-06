#include "Transform.hlsli"

struct VS_OUT
{
	float3 PosW : POSITION;
	float4 PosH : SV_POSITION;
};

VS_OUT main(float3 pos : POSITION)
{
	VS_OUT vso;
	vso.PosW = pos;

	matrix modelViewProjection = mul(model, mul(view, projection));

	vso.PosH = mul(float4(pos, 1.0f), modelViewProjection);
	return vso;
}