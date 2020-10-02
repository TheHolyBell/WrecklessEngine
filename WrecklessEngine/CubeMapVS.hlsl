#include "Transform.hlsli"

struct VS_OUT
{
	float3 PosL : POSITION;
	float3 PosW : POSITION1;
	float4 PosH : SV_POSITION;
};

VS_OUT main( float3 pos : POSITION )
{
	VS_OUT vso;

	vso.PosL = pos;

	matrix v = view;
	v[3] = float4(0, 0, 0, 1);
	matrix viewProjection = mul(v, projection);


	vso.PosW = mul(float4(pos, 1.0f), model).xyz;
	vso.PosH = mul(float4(pos, 1.0f), viewProjection).xyww;

	return vso;
}