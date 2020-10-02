cbuffer Buffie : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
}

cbuffer cbPerObject : register(b1)
{
	matrix g_ShadowTransform;
}

struct VertexInput
{
	float3 PosL : Position;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float3 Binormal : Bitangent;
	float2 TexCoords : Texcoord;
};

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

VertexOutput main(VertexInput vin)
{
	VertexOutput vout;

	matrix modelViewProjection = mul(model, mul(view, projection));
	vout.PosH = mul(float4(vin.PosL, 1.0f), modelViewProjection);

	vout.PosW = mul(float4(vin.PosL, 1.0f), model).xyz;
	//vout.PosH = mul(float4(vout.PosW, 1.0f), mul(view, projection));
	vout.Normal = vin.Normal;
	vout.Tangent = vin.Tangent;
	vout.Binormal = vin.Binormal;
	vout.TexCoords = vin.TexCoords;

	vout.ShadowPosH = mul(float4(vout.PosW, 1.0f), g_ShadowTransform);

	return vout;
}