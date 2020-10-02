struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	float2 TiledTex : TEXCOORD1;
	float4 ShadowPosH : TEXCOORD2;
};

Texture2D gLayerMap : register(t0);
Texture2D g_ShadowMap : register(t1);
Texture2D gHeightMap : register(t2);

SamplerState g_SamplerState : register(s0);
SamplerState g_ShadowSampler : register(s1);

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

cbuffer Buffie : register(b0)
{
	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	float gWorldCellSpace;
}

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
};

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

float2 texOffset(int u, int v, uint width, uint height)
{
	return float2(u * 1.0f / (float)width, v * 1.0f / (float)height);
}

float CalcShadowFactor(float4 shadowPosH)
{
	/*// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;

	if (shadowPosH.x < -1.0f || shadowPosH.x > 1.0f ||
		shadowPosH.y < -1.0f || shadowPosH.y > 1.0f ||
		shadowPosH.z < 0.0f || shadowPosH.z > 1.0f) return 0.0f;

	//transform clip space coords to texture space coords (-1:1 to 0:1)
	shadowPosH.x = shadowPosH.x / 2 + 0.5;
	shadowPosH.y = shadowPosH.y / -2 + 0.5;

	uint width;
	uint height;
	g_ShadowMap.GetDimensions(width, height);

	//apply shadow map bias
	shadowPosH.z -= 0.0005f;

	//PCF sampling for shadow map
	float sum = 0;
	float x, y;

	//perform PCF filtering on a 4 x 4 texel neighborhood
	for (y = -1.5; y <= 1.5; y += 1.0)
	{
		for (x = -1.5; x <= 1.5; x += 1.0)
		{
			sum += g_ShadowMap.SampleCmpLevelZero(cmpSampler, shadowPosH.xy + texOffset(x, y, width, height), shadowPosH.z);
		}
	}

	float shadowFactor = sum / 16.0;

	return shadowFactor;*/

	// perform perspective divide
	float3 projCoords = shadowPosH.xyz / shadowPosH.w;
	// transform to [0,1] range
	projCoords.x = shadowPosH.x / 2 + 0.5;
	projCoords.y = shadowPosH.y / -2 + 0.5;

	if (projCoords.x < -1.0f || projCoords.x > 1.0f ||
		projCoords.y < -1.0f || projCoords.y > 1.0f ||
		projCoords.z < 0.0f || projCoords.z > 1.0f) return 0.0f;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = g_ShadowMap.Sample(g_ShadowSampler, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	float bias = 0.005f;
	float shadow = 0.0;

	uint width;
	uint height;
	g_ShadowMap.GetDimensions(width, height);

	float2 texelSize;
	texelSize.x = 1.0f / (float)width;
	texelSize.y = 1.0f / (float)height;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = g_ShadowMap.Sample(g_SamplerState, projCoords.xy + float2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.5 : 0.0;
		}
	}
	shadow /= 9.0;


	return shadow;
}


float4 main(DomainOut pin) : SV_TARGET
{
	// Estimate normal and tangent using central differences

	/*float2 leftTex = pin.Tex + float2(-gTexelCellSpaceU, 0.0f);
	float2 rightTex = pin.Tex + float2(gTexelCellSpaceU, 0.0f);
	float2 bottomTex = pin.Tex + float2(0.0f, gTexelCellSpaceV);
	float2 topTex = pin.Tex + float2(0.0f, -gTexelCellSpaceV);

	float leftY = gHeightMap.SampleLevel(samHeightmap, leftTex, 0).r;
	float rightY = gHeightMap.SampleLevel(samHeightmap, rightTex, 0).r;
	float bottomY = gHeightMap.SampleLevel(samHeightmap, bottomTex, 0).r;
	float topY = gHeightMap.SampleLevel(samHeightmap, topTex, 0).r;

	float3 tangent = normalize(float3(2.0f * gWorldCellSpace, rightY - leftY, 0.0f));
	float3 bitan = normalize(float3(0.0f, bottomY - topY, -2.0f * gWorldCellSpace));
	float3 normalW = cross(tangent, bitan);

	// Sample layers in texture array.
	float4 c0 = gLayerMapArray.Sample(samLinear, float3(pin.TiledTex, 0.0f));
	float4 c1 = gLayerMapArray.Sample(samLinear, float3(pin.TiledTex, 1.0f));
	float4 c2 = gLayerMapArray.Sample(samLinear, float3(pin.TiledTex, 2.0f));
	float4 c3 = gLayerMapArray.Sample(samLinear, float3(pin.TiledTex, 3.0f));
	float4 c4 = gLayerMapArray.Sample(samLinear, float3(pin.TiledTex, 4.0f));

	// Sample the blend map
	float4 t = gBlendMap.Sample(samLinear, pin.Tex);

	// Blend the layers on top of each of other
	float4 texColor = c0;
	texColor = lerp(texColor, c1, t.r);
	texColor = lerp(texColor, c2, t.g);
	texColor = lerp(texColor, c3, t.b);
	texColor = lerp(texColor, c4, t.a);

	return texColor;*/

	float shadow = 1.0f - CalcShadowFactor(pin.ShadowPosH);
	float4 diffuse = gLayerMap.Sample(g_SamplerState, pin.TiledTex);
	diffuse.xyz *= shadow;
	return  diffuse;
}