struct VertexOutput
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BITANGENT;
	float2 TexCoords : TEXCOORD;
	float4 ShadowPosH : TEXCOORD1;
};

Texture2D g_Albedo : register(t0);
Texture2D g_ShadowMap : register(t1);
SamplerState g_SamplerState : register(s0);

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
};

float CalcShadowFactor(float4 shadowPosH, uniform SamplerComparisonState samShadow,
	uniform Texture2D shadowMap)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;

	uint width;
	uint height;
	shadowMap.GetDimensions(width, height);

	float smap_dx = 1.0f / (float)width;

	// Depth in NDC space.
	float depth = shadowPosH.z;

	// Texel size.
	const float dx = smap_dx;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}

float4 main(VertexOutput pin) : SV_TARGET
{
	float shadow = CalcShadowFactor(pin.ShadowPosH, samShadow, g_ShadowMap);
	return g_Albedo.Sample(g_SamplerState, pin.TexCoords) * shadow;
}