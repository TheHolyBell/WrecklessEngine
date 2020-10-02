Texture2D gHeightMap : register(t0);

cbuffer Buff : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
	float2 gTexScale;
}

SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

struct VertexOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 BoundsY  : TEXCOORD1;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = vin.PosL;
	vout.PosW.y = gHeightMap.SampleLevel(samHeightmap, vin.Tex, 0).r;
	vout.PosW = mul(float4(vout.PosW, 1.0f), model);
	vout.Tex = vin.Tex;
	vout.BoundsY = vin.BoundsY;


	return vout;
}