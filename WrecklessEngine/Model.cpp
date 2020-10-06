#include "Model.h"

#include "GlobalClock.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <filesystem>

#include "InputOutputStream.h"
#include "Manipulators.h"
#include "Renderer.h"

#include "SceneCamera.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Components.h"

#include "ShaderReflector.h"

static const uint32_t s_MeshImportFlags =
	aiProcess_Triangulate |
	aiProcess_ConvertToLeftHanded |
	aiProcess_GenNormals |
	aiProcess_CalcTangentSpace |
	aiProcess_JoinIdenticalVertices |
	aiProcess_GenUVCoords;

static UINT CalcConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + 255) & ~255;
}

struct DEFAULT_CB
{
	DirectX::XMMATRIX model;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct ANIMATION_CB
{
	DirectX::XMMATRIX BoneTransforms[100];
};

namespace Drawables
{
	Model::Model(unsigned int entID, const std::string& file_path)
		: m_FilePath(file_path), m_EntID(entID)
	{
		using namespace DirectX;
		using namespace Graphics;
		m_pImporter = std::make_unique<Assimp::Importer>();

		const aiScene* scene = m_pImporter->ReadFile(file_path, s_MeshImportFlags);
		if (scene == nullptr || !scene->HasMeshes())
			WRECK_ASSERT(false, "Failed to load mesh file");

		m_Scene = scene;

		m_IsAnimated = scene->mAnimations != nullptr;
		ShaderConfig config;
		if (m_IsAnimated)
		{
			config.vs_path = "Shaders/Bin/AnimatedVS.cso";
			config.ps_path = "Shaders/Bin/AnimatedPS.cso";
		}
		else
		{
			config.vs_path = "Shaders/Bin/DefaultVS.cso";
			config.ps_path = "Shaders/Bin/DefaultPS.cso";
		}
		
		SetShaders(config);
		

		auto transform = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&scene->mRootNode->mTransformation.a1));
		XMStoreFloat4x4(&m_InverseTransform, XMMatrixInverse(&XMMatrixDeterminant(transform), transform));

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		m_Submeshes.reserve(scene->mNumMeshes);
		for (size_t m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];

			Submesh& submesh = m_Submeshes.emplace_back();
			submesh.BaseVertex = vertexCount;
			submesh.BaseIndex = indexCount;
			submesh.MaterialIndex = mesh->mMaterialIndex;
			submesh.IndexCount = mesh->mNumFaces * 3;
			submesh.MeshName = mesh->mName.C_Str();

			vertexCount += mesh->mNumVertices;
			indexCount += submesh.IndexCount;

			WRECK_ASSERT(mesh->HasPositions(), "Meshes require positions.");
			WRECK_ASSERT(mesh->HasNormals(), "Meshes require normals.");

			// Vertices
			if (m_IsAnimated)
			{
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					AnimatedVertex vertex;
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					m_AnimatedVertices.push_back(vertex);
				}
			}
			else
			{
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex;
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					m_StaticVertices.push_back(vertex);
				}
			}

			// Indices
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				WRECK_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
				Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
				m_Indices.push_back(index.V1);
				m_Indices.push_back(index.V2);
				m_Indices.push_back(index.V3);

				if (!m_IsAnimated)
					m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
			}
		}

		TraverseNodes(scene->mRootNode);

		// Bones
		if (m_IsAnimated)
		{
			for (size_t m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* mesh = scene->mMeshes[m];
				Submesh& submesh = m_Submeshes[m];

				for (size_t i = 0; i < mesh->mNumBones; i++)
				{
					aiBone* bone = mesh->mBones[i];
					std::string boneName(bone->mName.data);
					int boneIndex = 0;

					if (m_BoneMapping.find(boneName) == m_BoneMapping.end())
					{
						// Allocate an index for a new bone
						boneIndex = m_BoneCount;
						m_BoneCount++;
						BoneInfo bi;
						m_BoneInfo.push_back(bi);
						DirectX::XMStoreFloat4x4(&m_BoneInfo[boneIndex].BoneOffset, DirectX::XMMatrixTranspose(XMMATRIX(&bone->mOffsetMatrix.a1)));
						m_BoneMapping[boneName] = boneIndex;
					}
					else
					{
						IO::cout << "Found existing bone in map" << IO::endl;
						boneIndex = m_BoneMapping[boneName];
					}

					for (size_t j = 0; j < bone->mNumWeights; j++)
					{
						int VertexID = submesh.BaseVertex + bone->mWeights[j].mVertexId;
						float Weight = bone->mWeights[j].mWeight;
						m_AnimatedVertices[VertexID].AddBoneData(boneIndex, Weight);
					}
				}
			}
		}

		// Materials
		if (scene->HasMaterials())
		{
			for (int i = 0; i < scene->mNumMaterials; ++i)
			{
				auto aiMaterial = scene->mMaterials[i];

				aiString aiTexPath;
				uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
				bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
				if (hasAlbedoMap)
				{
					std::filesystem::path path = file_path;
					auto parentPath = path.parent_path();
					parentPath /= std::string(aiTexPath.data);
					std::string texturePath = parentPath.string();

					m_Textures[i] = Bindable::Texture2D::Resolve(texturePath);
				}
			}
		}


		if (m_IsAnimated)
		{
			m_VertexBuffer = Renderer::GetDevice()->CreateVertexBuffer(m_AnimatedVertices.data(), sizeof(AnimatedVertex) * m_AnimatedVertices.size());
		}
		else
		{
			m_VertexBuffer = Renderer::GetDevice()->CreateVertexBuffer(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
		}

		m_IndexBuffer = Renderer::GetDevice()->CreateIndexBuffer(m_Indices);


		m_DefaultCBuffer = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(DEFAULT_CB)));
		m_AnimationCBuffer = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(ANIMATION_CB)));


		SAMPLER_DESC sampDesc = {};
		sampDesc.AddressU = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
		sampDesc.Filter = SAMPLE_FILTER::ANISOTROPIC;
		sampDesc.MaxAnisotropy = 16;
		sampDesc.MaxLOD = FLT_MAX;
		m_AnisotropicSampler = Renderer::GetDevice()->CreateSamplerState(sampDesc);
	}
	Model::~Model()
	{
	}
	void Model::Update()
	{
		if (m_IsAnimated)
		{
			if (m_AnimationPlaying)
			{
				float delta = Profiling::GlobalClock::DeltaTime();
				m_WorldTime += delta;

				float ticksPerSecond = (float)(m_Scene->mAnimations[0]->mTicksPerSecond != 0 ? m_Scene->mAnimations[0]->mTicksPerSecond : 25.0f) * 1;
				m_AnimationTime += delta * ticksPerSecond;
				m_AnimationTime = fmod(m_AnimationTime, (float)m_Scene->mAnimations[0]->mDuration);
			}

			BoneTransform(m_AnimationTime);

			ANIMATION_CB animationCB;
			for (int i = 0; i < m_BoneCount; ++i)
			{
				animationCB.BoneTransforms[i] = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_BoneTransforms[i]));
			}

			using namespace Graphics;
			Renderer::GetRenderContext()->MapDataToBuffer(m_AnimationCBuffer, &animationCB, sizeof(animationCB));
		}
	}
	void Model::Draw()
	{
		using namespace Graphics;
		ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());

		Renderer::GetRenderContext()->BindVertexShader(m_VertexShader);
		Renderer::GetRenderContext()->BindPixelShader(m_PixelShader);
		Renderer::GetRenderContext()->BindInputLayout(m_InputLayout);

		Renderer::GetRenderContext()->BindVertexBuffer(m_VertexBuffer, m_IsAnimated ? sizeof(AnimatedVertex) : sizeof(Vertex), 0);
		Renderer::GetRenderContext()->BindIndexBuffer(m_IndexBuffer, 0);

		Renderer::GetRenderContext()->BindConstantBuffer(m_DefaultCBuffer, SHADER_TYPE::Vertex, 0);
		Renderer::GetRenderContext()->BindConstantBuffer(m_AnimationCBuffer, SHADER_TYPE::Vertex, 1);

		Renderer::GetRenderContext()->BindSamplerState(m_AnisotropicSampler, 0);

		deviceContext->RSSetState(nullptr);
		deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
		deviceContext->OMSetDepthStencilState(nullptr, 0xFFFFFFFF);

		for (auto& subMesh : m_Submeshes)
		{
			{
				DEFAULT_CB cbData;
				cbData.model = DirectX::XMMatrixIdentity();

				ECS::Entity entity = ECS::SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)m_EntID);
				if (entity.HasComponent<ECS::TransformComponent>())
					cbData.model = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&subMesh.Transform) * DirectX::XMLoadFloat4x4(&entity.GetComponent<ECS::TransformComponent>().Transform));

				cbData.view = DirectX::XMMatrixTranspose(CameraSystem::SceneCamera::GetView());
				cbData.projection = DirectX::XMMatrixTranspose(CameraSystem::SceneCamera::GetProjection());

				Renderer::GetRenderContext()->MapDataToBuffer(m_DefaultCBuffer, &cbData, sizeof(DEFAULT_CB));

				if (m_Textures[subMesh.MaterialIndex] != nullptr)
					m_Textures[subMesh.MaterialIndex]->Bind();
			}
			Renderer::GetRenderContext()->DrawIndexed(subMesh.IndexCount, subMesh.BaseIndex, subMesh.BaseVertex);
		}
	}
	void Model::SetShaders(ShaderConfig config)
	{
		using namespace Graphics;

		m_VertexShader = Renderer::GetDevice()->CreateVertexShader(config.vs_path);
		m_PixelShader = Renderer::GetDevice()->CreatePixelShader(config.ps_path);

		m_InputLayout = Renderer::GetDevice()->CreateInputLayout(ShaderReflector::GetLayoutFromShader((ID3DBlob*)m_VertexShader->GetByteCode()), m_VertexShader->GetByteCode());
	}
	void Model::BoneTransform(float time)
	{
		ReadNodeHierarchy(time, m_Scene->mRootNode, DirectX::XMMatrixIdentity());
		m_BoneTransforms.resize(m_BoneCount);
		for (size_t i = 0; i < m_BoneCount; i++)
			m_BoneTransforms[i] = m_BoneInfo[i].FinalTransform;
	}
	void Model::ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const DirectX::XMMATRIX& ParentTransform)
	{
		using namespace DirectX;
		std::string name(pNode->mName.data);
		const aiAnimation* animation = m_Scene->mAnimations[0];
		XMMATRIX nodeTransform(DirectX::XMMatrixTranspose(XMMATRIX(&pNode->mTransformation.a1)));

		const aiNodeAnim* nodeAnim = FindNodeAnim(animation, name);

		if (nodeAnim)
		{
			XMFLOAT3 scale = InterpolateScale(AnimationTime, nodeAnim);
			XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

			Quat rotation = InterpolateRotation(AnimationTime, nodeAnim);
			XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));

			XMFLOAT3 translation = InterpolateTranslation(AnimationTime, nodeAnim);
			XMMATRIX translationMatrix = XMMatrixTranslation(translation.x, translation.y, translation.z);

			nodeTransform = scaleMatrix * rotationMatrix * translationMatrix;
		}

		XMMATRIX transform = nodeTransform * ParentTransform;

		if (m_BoneMapping.find(name) != m_BoneMapping.end())
		{
			uint32_t BoneIndex = m_BoneMapping[name];
			XMStoreFloat4x4(&m_BoneInfo[BoneIndex].FinalTransform, XMLoadFloat4x4(&m_BoneInfo[BoneIndex].BoneOffset) * transform * XMLoadFloat4x4(&m_InverseTransform));
		}

		for (uint32_t i = 0; i < pNode->mNumChildren; i++)
			ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], transform);
	}
	void Model::TraverseNodes(aiNode* node, const DirectX::XMMATRIX& parentTransform, uint32_t level)
	{
		using namespace DirectX;
		XMMATRIX transform = DirectX::XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransform;
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			uint32_t mesh = node->mMeshes[i];
			auto& submesh = m_Submeshes[mesh];
			submesh.NodeName = node->mName.C_Str();
			XMStoreFloat4x4(&submesh.Transform,transform);
		}


		for (uint32_t i = 0; i < node->mNumChildren; i++)
			TraverseNodes(node->mChildren[i], transform, level + 1);
	}
	const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName)
				return nodeAnim;
		}
		return nullptr;
	}
	uint32_t Model::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	uint32_t Model::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		WRECK_ASSERT(pNodeAnim->mRotationKeys > 0, "Rotation keys are less or equal zero");

		for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	uint32_t Model::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		WRECK_ASSERT(pNodeAnim->mNumScalingKeys > 0, "");

		for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
				return i;
		}

		return 0;
	}
	DirectX::XMFLOAT3 Model::InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumPositionKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mPositionKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
		uint32_t NextPositionIndex = (PositionIndex + 1);
		WRECK_ASSERT(NextPositionIndex < nodeAnim->mNumPositionKeys, "");
		float DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		WRECK_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
		Factor = std::clamp(Factor, 0.0f, 1.0f);
		const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		auto aiVec = Start + Factor * Delta;
		return { aiVec.x, aiVec.y, aiVec.z };
	}
	Quat Model::InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumRotationKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mRotationKeys[0].mValue;
			return { v.x, v.y, v.z, v.w };
		}

		uint32_t RotationIndex = FindRotation(animationTime, nodeAnim);
		uint32_t NextRotationIndex = (RotationIndex + 1);
		WRECK_ASSERT(NextRotationIndex < nodeAnim->mNumRotationKeys, "");
		float DeltaTime = (float)(nodeAnim->mRotationKeys[NextRotationIndex].mTime - nodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (animationTime - (float)nodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		WRECK_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
		Factor = std::clamp(Factor, 0.0f, 1.0f);
		
		/*const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
		auto q = aiQuaternion();
		aiQuaternion::Interpolate(q, StartRotationQ, EndRotationQ, Factor);
		q = q.Normalize();*/

		using namespace DirectX;
		const aiQuaternion& aStartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& aEndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
		
		XMVECTOR StartRotationQ = XMVectorSet(aStartRotationQ.x, aStartRotationQ.y, aStartRotationQ.z, aStartRotationQ.w);
		XMVECTOR EndRotationQ = XMVectorSet(aEndRotationQ.x, aEndRotationQ.y, aEndRotationQ.z, aEndRotationQ.w);

		XMFLOAT4 q;
		XMStoreFloat4(&q, DirectX::XMQuaternionSlerp(StartRotationQ, EndRotationQ, Factor));

		return q;
	}
	DirectX::XMFLOAT3 Model::InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim)
	{
		if (nodeAnim->mNumScalingKeys == 1)
		{
			// No interpolation necessary for single value
			auto v = nodeAnim->mScalingKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		uint32_t index = FindScaling(animationTime, nodeAnim);
		uint32_t nextIndex = (index + 1);
		WRECK_ASSERT(nextIndex < nodeAnim->mNumScalingKeys, "");
		float deltaTime = (float)(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
		float factor = (animationTime - (float)nodeAnim->mScalingKeys[index].mTime) / deltaTime;
		WRECK_ASSERT(factor <= 1.0f, "Factor must be below 1.0f");
		factor = std::clamp(factor, 0.0f, 1.0f);
		const auto& start = nodeAnim->mScalingKeys[index].mValue;
		const auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
		auto delta = end - start;
		auto aiVec = start + factor * delta;
		return { aiVec.x, aiVec.y, aiVec.z };
	}
}