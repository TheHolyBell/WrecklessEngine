#define MAX_BONES 100
cbuffer Buff : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

cbuffer AnimationBuff : register(b1)
{
	matrix g_BoneTransforms[MAX_BONES];
};

struct VertexInput
{
	float3 PosL : Position;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float3 Binormal : Bitangent;
	float2 TexCoords : Texcoord;
	uint4 BoneIndices : BONEINDICES;
	float4 BoneWeights : BONEWEIGHTS;
};

struct VertexOutput
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BITANGENT;
	float2 TexCoords : TEXCOORD;
};

VertexOutput main(VertexInput vin)
{
	/*VertexOutput vout;

	matrix boneTransform = mul(vin.BoneWeights[0], g_BoneTransforms[vin.BoneIndices[0]]);

	boneTransform += mul(vin.BoneWeights[1], g_BoneTransforms[vin.BoneIndices[1]]);
	boneTransform += mul(vin.BoneWeights[2], g_BoneTransforms[vin.BoneIndices[2]]);
	boneTransform += mul(vin.BoneWeights[3], g_BoneTransforms[vin.BoneIndices[3]]);

	float4 localPosition = mul(float4(vin.PosL, 1.0f), boneTransform);

	matrix modelViewProjection = mul(model, mul(view, projection));

	vout.PosH = mul(localPosition, modelViewProjection);
	vout.PosW = mul(localPosition, model).xyz;
	vout.Normal = vin.Normal;
	vout.Tangent = vin.Tangent;
	vout.Binormal = vin.Binormal;
	vout.TexCoords = vin.TexCoords;

	return vout;*/	

	float4 localPosition = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	float3 tangentL = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		// Assume no nonuniform scaling when transforming normals, so 
		// that we do not have to use the inverse-transpose.

		localPosition += vin.BoneWeights[i] * mul(float4(vin.PosL, 1.0f), g_BoneTransforms[vin.BoneIndices[i]]);
	}

	matrix modelViewProjection = mul(model, mul(view, projection));

	VertexOutput vout;
	vout.PosH = mul(localPosition, modelViewProjection);
	vout.PosW = mul(localPosition, model).xyz;
	vout.Normal = vin.Normal;
	vout.Tangent = vin.Tangent;
	vout.Binormal = vin.Binormal;
	vout.TexCoords = vin.TexCoords;

	return vout;
}