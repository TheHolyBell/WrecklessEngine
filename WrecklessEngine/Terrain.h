#pragma once

#include "CommonInclude.h"
#include "IMesh.h"
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "IBuffer.h"
#include "IShader.h"
#include "ISamplerState.h"

namespace Drawables
{
	class Terrain : public IMesh
	{
	public:
		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT2 Tex;
			DirectX::XMFLOAT2 BoundsY;
		};

		struct InitInfo
		{
			std::wstring HeightMapFilename;
			std::wstring LayerMapFilename0;
			std::wstring LayerMapFilename1;
			std::wstring LayerMapFilename2;
			std::wstring LayerMapFilename3;
			std::wstring LayerMapFilename4;
			std::wstring BlendMapFilename;
			float HeightScale;
			UINT HeightmapWidth;
			UINT HeightmapHeight;
			float CellSpacing;
		};
	public:
		Terrain(unsigned entID);
		~Terrain() = default;

		float GetWidth() const;
		float GetDepth() const;
		float GetHeight(float x, float z) const;

		virtual void Update() override;
		virtual void Draw() override;

	private:
		void LoadHeightMap();
		void Smooth();
		bool InBounds(int i, int j);
		float Average(int i, int j);
		void CalcAllPatchBoundsY();
		void CalcPatchBoundsY(UINT i, UINT j);
		void BuildQuadPatchVB();
		void BuildShaders();
		void BuildInputLayout(ID3D11Device* device);
		void BuildTerrainCB();
		void BuildQuadPatchIB();
		void BuildHeightmapSRV(ID3D11Device* device);

	private:
		// Divide heightmap into patches such that each patch has CellsPerPatch cells
		// and CellsPerPatch+1 vertices.  Use 64 so that if we tessellate all the way 
		// to 64, we use all the data from the heightmap. 
		static const int CellsPerPatch = 64;

		Ref<Graphics::IVertexBuffer> m_pQuadPatchVB;
		Ref<Graphics::IIndexBuffer>  m_pQuadPatchIB;

		Ref<Graphics::IConstantBuffer> m_pTerrainCBHS;
		Ref<Graphics::IConstantBuffer> m_pTerrainCBDS;
		Ref<Graphics::IConstantBuffer> m_pTerrainCBPS;

		Ref<Graphics::IConstantBuffer> m_pShadowCB;

		Ref<Graphics::ISamplerState> m_pSamplerState;
		Ref<Graphics::ISamplerState> m_pShadowSampler;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mLayerMapArraySRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBlendMapSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mHeightMapSRV;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

		Ref<Graphics::IVertexShader> m_pVertexShader;
		Ref<Graphics::IHullShader> m_pHullShader;
		Ref<Graphics::IDomainShader> m_pDomainShader;
		Ref<Graphics::IPixelShader> m_pPixelShader;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mWireframeRasterizer;

		InitInfo mInfo;

		UINT mNumPatchVertices = 0;
		UINT mNumPatchQuadFaces = 0;

		UINT mIndices = 0;

		UINT mNumPatchVertRows = 0;
		UINT mNumPatchVertCols = 0;
		std::vector<DirectX::XMFLOAT2> mPatchBoundsY;
		std::vector<float> mHeightmap;

		unsigned mEntID = -1;
	};
}