#pragma once
#include "IMesh.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <string>

#include "Texture2D.h"
#include "IBuffer.h"
#include "IInputLayout.h"
#include "IShader.h"
#include "ISamplerState.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
	class Importer;
}

namespace Drawables
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT3 Binormal;
		DirectX::XMFLOAT2 TexCoord;
	};

	struct AnimatedVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT3 Binormal;
		DirectX::XMFLOAT2 TexCoord;

		uint32_t IDs[4] = { 0, 0,0, 0 };
		float Weights[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

		void AddBoneData(uint32_t BoneID, float Weight)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (Weights[i] == 0.0)
				{
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}

			WRECK_ASSERT(false, "Vertex has more than four bones/weights");
		}
	};

	static const int NumAttributes = 5;

	struct Index
	{
		uint32_t V1, V2, V3;
	};

	static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

	struct BoneInfo
	{
		DirectX::XMFLOAT4X4 BoneOffset;
		DirectX::XMFLOAT4X4 FinalTransform;
	};

	struct VertexBoneData
	{
		uint32_t IDs[4];
		float Weights[4];

		VertexBoneData()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		};

		void AddBoneData(uint32_t BoneID, float Weight)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (Weights[i] == 0.0)
				{
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}

			// should never get here - more bones than we have space for
			WRECK_ASSERT(false, "Too many bones!");
		}
	};

	struct Triangle
	{
		Vertex V0, V1, V2;
		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
			: V0(v0), V1(v1), V2(v2) {}
	};

	class Submesh
	{
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;

		DirectX::XMFLOAT4X4 Transform;

		std::string NodeName, MeshName;
	};

	static constexpr DirectX::XMFLOAT4X4 IdentityMatrix()
	{
		return DirectX::XMFLOAT4X4
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	using Quat = DirectX::XMFLOAT4;

	struct ShaderConfig
	{
		std::string vs_path;
		std::string ps_path;
	};

	class Model : public IMesh
	{
	public:
		Model(unsigned int entID, const std::string& file_path);
		~Model();

		virtual void Update() override;
		virtual void Draw() override;

		void SetShaders(ShaderConfig config);

		const std::string& GetFilePath() const { return m_FilePath; }
	private:
		void BoneTransform(float time);
		void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const DirectX::XMMATRIX& ParentTransform);
		void TraverseNodes(aiNode* node, const DirectX::XMMATRIX& parentTransform = DirectX::XMMatrixIdentity(), uint32_t level = 0);

		const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);
		uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		DirectX::XMFLOAT3 InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim);
		Quat InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim);
		DirectX::XMFLOAT3 InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim);
	private:
		std::vector<Submesh> m_Submeshes;

		std::unique_ptr<Assimp::Importer> m_pImporter;

		DirectX::XMFLOAT4X4 m_InverseTransform;

		uint32_t m_BoneCount = 0;
		std::vector<BoneInfo> m_BoneInfo;

		Ref<Graphics::IVertexShader> m_VertexShader;
		Ref<Graphics::IPixelShader> m_PixelShader;
		Ref<Graphics::IInputLayout> m_InputLayout;
		Ref<Graphics::ISamplerState> m_AnisotropicSampler;

		/*Ref<Graphics::IVertexBuffer> m_VertexBuffer;
		Ref<Graphics::IIndexBuffer> m_IndexBuffer;
		Ref<Graphics::IConstantBuffer> m_ConstantBuffer;*/

		Ref<Graphics::IVertexBuffer> m_VertexBuffer;
		Ref<Graphics::IIndexBuffer> m_IndexBuffer;
		Ref<Graphics::IConstantBuffer> m_DefaultCBuffer;
		Ref<Graphics::IConstantBuffer> m_AnimationCBuffer;

		std::vector<Vertex> m_StaticVertices;
		std::vector<AnimatedVertex> m_AnimatedVertices;
		std::vector<unsigned int> m_Indices;
		std::unordered_map<std::string, uint32_t> m_BoneMapping;
		std::vector<DirectX::XMFLOAT4X4> m_BoneTransforms;
		const aiScene* m_Scene;

		std::unordered_map<unsigned int, Ref<Bindable::Texture2D>> m_Textures;
		std::unordered_map<unsigned int, Ref<Bindable::Texture2D>> m_NormalMaps;

		std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

		bool m_IsAnimated = false;
		float m_AnimationTime = 0.0f;
		float m_WorldTime = 0.0f;
		bool m_AnimationPlaying = true;

		std::string m_FilePath;

		unsigned int m_EntID;
	};
}