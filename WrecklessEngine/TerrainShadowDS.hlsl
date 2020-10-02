cbuffer Buff : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
	float2 gTexScale;
}

cbuffer cbPerObject : register(b1)
{
	matrix g_ShadowTransform;
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 TiledTex : TEXCOORD1;
	float4 ShadowPosH : TEXCOORD2;
};

struct HullOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
};

Texture2D gHeightMap : register(t0);

SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("quad")]
DomainOut main(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	// Bilinear interpolation.
	dout.PosW = lerp(
		lerp(quad[0].PosW, quad[1].PosW, uv.x),
		lerp(quad[2].PosW, quad[3].PosW, uv.x),
		uv.y);

	dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y);

	// Tile layer textures over terrain.
	dout.TiledTex = dout.Tex * gTexScale;

	// Displacement mapping
	dout.PosW.y = gHeightMap.SampleLevel(samHeightmap, dout.Tex, 0).r;

	// NOTE: We tried computing the normal in the shader using finite difference, 
	// but the vertices move continuously with fractional_even which creates
	// noticable light shimmering artifacts as the normal changes.  Therefore,
	// we moved the calculation to the pixel shader.  

	dout.ShadowPosH = mul(float4(dout.PosW, 1.0f), g_ShadowTransform);
	matrix worldViewProj = mul(model, mul(view, projection));

	// Project to homogeneous clip space.
	dout.PosH = mul(float4(dout.PosW, 1.0f), worldViewProj);


	return dout;
}