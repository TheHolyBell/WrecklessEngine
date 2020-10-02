#include "Terrain.h"
#include "Renderer.h"

#include <fstream>

#include "DXGIInfoManager.h"

#include <DirectXPackedVector.h>

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include "SceneManager.h"
#include "Components.h"

#include "SceneCamera.h"
#include "Entity.h"

#include "ShadowPass.h"

#include <d3dcompiler.h>

using namespace DirectX;

#include "FileHelper.h"
#include "StringHelper.h"
#include "Manipulators.h"
#include "GamePad.h"

static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture2DArraySRV(
	ID3D11Device* device, ID3D11DeviceContext* context,
	std::vector<std::wstring>& filenames)
{
	UINT size = filenames.size();

	std::vector<ID3D11Texture2D*> srcTex(size);

	for (int i = 0; i < size; ++i)
	{
		DirectX::CreateDDSTextureFromFile(device, filenames[i].c_str(),
			(ID3D11Resource**)&srcTex[i], nullptr);
	}

	D3D11_TEXTURE2D_DESC elemDesc = {};
	srcTex[0]->GetDesc(&elemDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc = {};
	texArrayDesc.Width = elemDesc.Width;
	texArrayDesc.Height = elemDesc.Height;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = elemDesc.Format;
	elemDesc.SampleDesc.Count = 1;
	elemDesc.SampleDesc.Quality = 0;
	elemDesc.Usage = D3D11_USAGE_DEFAULT;

	return nullptr;
}


void ExtractFrustumPlanes(XMFLOAT4* planes, CXMMATRIX M)
{
	
	//
	// Left
	//
	planes[0].x = M(0, 3) + M(0, 0);
	planes[0].y = M(1, 3) + M(1, 0);
	planes[0].z = M(2, 3) + M(2, 0);
	planes[0].w = M(3, 3) + M(3, 0);

	//
	// Right
	//
	planes[1].x = M(0, 3) - M(0, 0);
	planes[1].y = M(1, 3) - M(1, 0);
	planes[1].z = M(2, 3) - M(2, 0);
	planes[1].w = M(3, 3) - M(3, 0);

	//
	// Bottom
	//
	planes[2].x = M(0, 3) + M(0, 1);
	planes[2].y = M(1, 3) + M(1, 1);
	planes[2].z = M(2, 3) + M(2, 1);
	planes[2].w = M(3, 3) + M(3, 1);

	//
	// Top
	//
	planes[3].x = M(0, 3) - M(0, 1);
	planes[3].y = M(1, 3) - M(1, 1);
	planes[3].z = M(2, 3) - M(2, 1);
	planes[3].w = M(3, 3) - M(3, 1);

	//
	// Near
	//
	planes[4].x = M(0, 2);
	planes[4].y = M(1, 2);
	planes[4].z = M(2, 2);
	planes[4].w = M(3, 2);

	//
	// Far
	//
	planes[5].x = M(0, 3) - M(0, 2);
	planes[5].y = M(1, 3) - M(1, 2);
	planes[5].z = M(2, 3) - M(2, 2);
	planes[5].w = M(3, 3) - M(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i], v);
	}
}

static UINT CalcConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + 255) & ~255;
}

struct TerrainCBHS
{
	XMFLOAT3 gEyePosW;
	float gMinDist;
	XMFLOAT4 gWorldFrustumPlanes[6];
	float gMaxDist;
	float gMaxTess;
	float gMinTess;
};

struct TerrainCBDS
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX projection;
	XMFLOAT2 gTexScale;
};

struct TerrainCBPS
{
	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	float gWorldCellSpace;
	float Padding;
};

struct ShadowCB
{
	XMMATRIX shadowTransform;
};

namespace Drawables
{
	float Terrain::GetWidth() const
	{
		// Total terrain width.
		return (mInfo.HeightmapWidth - 1) * mInfo.CellSpacing;
	}
	float Terrain::GetDepth() const
	{
		// Total terrain depth.
		return (mInfo.HeightmapHeight - 1) * mInfo.CellSpacing;
	}
	float Terrain::GetHeight(float x, float z) const
	{
		// Transform from terrain local space to "cell" space.
		float c = (x + 0.5f * GetWidth()) / mInfo.CellSpacing;
		float d = (z - 0.5f * GetDepth()) / -mInfo.CellSpacing;

		// Get the row and column we are in.
		int row = (int)floorf(d);
		int col = (int)floorf(c);

		// Grab the heights of the cell we are in.
		// A*--*B
		//  | /|
		//  |/ |
		// C*--*D
		float A = mHeightmap[row * mInfo.HeightmapWidth + col];
		float B = mHeightmap[row * mInfo.HeightmapWidth + col + 1];
		float C = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col];
		float D = mHeightmap[(row + 1) * mInfo.HeightmapWidth + col + 1];

		// Where we are relative to the cell.
		float s = c - (float)col;
		float t = d - (float)row;

		// If upper triangle ABC.
		if (s + t <= 1.0f)
		{
			float uy = B - A;
			float vy = C - A;
			return A + s * uy + t * vy;
		}
		else // lower triangle DCB.
		{
			float uy = C - D;
			float vy = B - D;
			return D + (1.0f - s) * uy + (1.0f - t) * vy;
		}
	}
	void Terrain::Update()
	{
		using namespace Graphics;
		ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Graphics::Renderer::GetRenderContext()->GetNativePointer());
		using namespace ECS;

		Entity entity = SceneManager::GetActiveScene()->GetEntityByIndex((entt::entity)mEntID);
		
		auto model = DirectX::XMMatrixIdentity();

		if (entity.HasComponent<TransformComponent>())
			model = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&entity.GetComponent<TransformComponent>().Transform));

		auto view = CameraSystem::SceneCamera::GetView();
		auto projection = CameraSystem::SceneCamera::GetProjection();

		TerrainCBHS hsBuffer;
		hsBuffer.gEyePosW = CameraSystem::SceneCamera::GetPosition();
		hsBuffer.gMaxDist = 500.0f;
		hsBuffer.gMinDist = 20.0f;
		hsBuffer.gMinTess = 0.0f;
		hsBuffer.gMaxTess = 6.0f;
		
		XMFLOAT4 worldPlanes[6];
		ExtractFrustumPlanes(worldPlanes, view * projection);

		for (int i = 0; i < 6; ++i)
		{
			hsBuffer.gWorldFrustumPlanes[i] = worldPlanes[i];
		}

		Renderer::GetRenderContext()->MapDataToBuffer(m_pTerrainCBHS, &hsBuffer, sizeof(hsBuffer));
	
		TerrainCBDS dsBuffer;
		dsBuffer.gTexScale = { 50,50 };
		dsBuffer.model = model;
		dsBuffer.view = DirectX::XMMatrixTranspose(view);
		dsBuffer.projection = DirectX::XMMatrixTranspose(projection);

		Renderer::GetRenderContext()->MapDataToBuffer(m_pTerrainCBDS, &dsBuffer, sizeof(dsBuffer));

		TerrainCBPS psbuffer;
		psbuffer.gTexelCellSpaceU = 1.0f / mInfo.HeightmapWidth;
		psbuffer.gTexelCellSpaceV = 1.0f / mInfo.HeightmapHeight;
		psbuffer.gWorldCellSpace = mInfo.CellSpacing;

		Renderer::GetRenderContext()->MapDataToBuffer(m_pTerrainCBPS, &psbuffer, sizeof(psbuffer));

		ShadowCB shadowBuff;
		shadowBuff.shadowTransform = DirectX::XMMatrixTranspose(ShadowPass::GetShadowTransform());

		Renderer::GetRenderContext()->MapDataToBuffer(m_pShadowCB, &shadowBuff, sizeof(shadowBuff));
	}
	void Terrain::Draw()
	{
		using namespace Graphics;
		Update();
		
		ID3D11Device* device = reinterpret_cast<ID3D11Device*>(Graphics::Renderer::GetDevice()->GetNativePointer());
		ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Graphics::Renderer::GetRenderContext()->GetNativePointer());
	
		Renderer::GetRenderContext()->BindTopology(PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		deviceContext->IASetInputLayout(mInputLayout.Get());

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			deviceContext->RSSetState(mWireframeRasterizer.Get());
		else
			deviceContext->RSSetState(nullptr);
		deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
		deviceContext->OMSetDepthStencilState(nullptr, 0);

		Renderer::GetRenderContext()->BindVertexShader(m_pVertexShader);
		Renderer::GetRenderContext()->BindHullShader(m_pHullShader);
		Renderer::GetRenderContext()->BindDomainShader(m_pDomainShader);
		Renderer::GetRenderContext()->BindPixelShader(m_pPixelShader);

		Renderer::GetRenderContext()->BindSamplerState(m_pSamplerState, 0);
		Renderer::GetRenderContext()->BindSamplerState(m_pShadowSampler, 1);

		Renderer::GetRenderContext()->BindConstantBuffer(m_pTerrainCBHS, SHADER_TYPE::Hull, 0);
		Renderer::GetRenderContext()->BindConstantBuffer(m_pTerrainCBDS, SHADER_TYPE::Vertex, 0);
		Renderer::GetRenderContext()->BindConstantBuffer(m_pTerrainCBDS, SHADER_TYPE::Domain, 0);
		Renderer::GetRenderContext()->BindConstantBuffer(m_pShadowCB, SHADER_TYPE::Domain, 1);
		Renderer::GetRenderContext()->BindConstantBuffer(m_pTerrainCBPS, SHADER_TYPE::Pixel, 0);


		Renderer::GetRenderContext()->BindVertexBuffer(m_pQuadPatchVB, sizeof(Vertex), 0);
		Renderer::GetRenderContext()->BindIndexBuffer(m_pQuadPatchIB, 0);

		Renderer::GetRenderContext()->BindTexture2D(ShadowPass::GetDepthStencilSRV(), SHADER_TYPE::Pixel, 1);

		deviceContext->VSSetShaderResources(0, 1, mHeightMapSRV.GetAddressOf());
		deviceContext->DSSetShaderResources(0, 1, mHeightMapSRV.GetAddressOf());

		deviceContext->PSSetShaderResources(0, 1, mLayerMapArraySRV.GetAddressOf());
		//deviceContext->PSSetShaderResources(1, 1, mBlendMapSRV.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, mHeightMapSRV.GetAddressOf());
	
		Renderer::GetRenderContext()->DrawIndexed(mNumPatchQuadFaces * 4, 0, 0);

		Renderer::GetRenderContext()->BindHullShader(nullptr);
		Renderer::GetRenderContext()->BindDomainShader(nullptr);
	}
	void Terrain::LoadHeightMap()
	{
		// A height for each vertex
		std::vector<unsigned char> in(mInfo.HeightmapWidth * mInfo.HeightmapHeight);

		// Open the file.
		std::ifstream inFile;
		inFile.open(mInfo.HeightMapFilename.c_str(), std::ios_base::binary);

		if (inFile)
		{
			// Read the RAW bytes.
			inFile.read((char*)&in[0], (std::streamsize)in.size());

			// Done with file.
			inFile.close();
		}

		// Copy the array data into a float array and scale it.
		mHeightmap.resize(mInfo.HeightmapHeight * mInfo.HeightmapWidth, 0);
		for (UINT i = 0; i < mInfo.HeightmapHeight * mInfo.HeightmapWidth; ++i)
		{
			mHeightmap[i] = (in[i] / 255.0f) * mInfo.HeightScale;
		}
	}
	void Terrain::Smooth()
	{
		std::vector<float> dest(mHeightmap.size());

		for (UINT i = 0; i < mInfo.HeightmapHeight; ++i)
		{
			for (UINT j = 0; j < mInfo.HeightmapWidth; ++j)
			{
				dest[i * mInfo.HeightmapWidth + j] = Average(i, j);
			}
		}

		// Replace the old heightmap with the filtered one.
		mHeightmap = dest;
	}
	bool Terrain::InBounds(int i, int j)
	{
		// True if ij are valid indices; false otherwise.
		return
			i >= 0 && i < (int)mInfo.HeightmapHeight&&
			j >= 0 && j < (int)mInfo.HeightmapWidth;
	}
	float Terrain::Average(int i, int j)
	{
		// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

		float avg = 0.0f;
		float num = 0.0f;

		// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
		// and no iterations of the outer for loop occur.
		for (int m = i - 1; m <= i + 1; ++m)
		{
			for (int n = j - 1; n <= j + 1; ++n)
			{
				if (InBounds(m, n))
				{
					avg += mHeightmap[m * mInfo.HeightmapWidth + n];
					num += 1.0f;
				}
			}
		}

		return avg / num;
	}
	void Terrain::CalcAllPatchBoundsY()
	{
		mPatchBoundsY.resize(mNumPatchQuadFaces);

		// For each patch
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				CalcPatchBoundsY(i, j);
			}
		}
	}
	void Terrain::CalcPatchBoundsY(UINT i, UINT j)
	{
		// Scan the heightmap values this patch covers and compute the min/max height.

		UINT x0 = j * CellsPerPatch;
		UINT x1 = (j + 1) * CellsPerPatch;

		UINT y0 = i * CellsPerPatch;
		UINT y1 = (i + 1) * CellsPerPatch;

		float minY = FLT_MAX;
		float maxY = -FLT_MAX;
		for (UINT y = y0; y <= y1; ++y)
		{
			for (UINT x = x0; x <= x1; ++x)
			{
				UINT k = y * mInfo.HeightmapWidth + x;
				minY = std::min(minY, mHeightmap[k]);
				maxY = std::max(maxY, mHeightmap[k]);
			}
		}

		UINT patchID = i * (mNumPatchVertCols - 1) + j;
		mPatchBoundsY[patchID] = XMFLOAT2(minY, maxY);
	}
	void Terrain::BuildQuadPatchVB()
	{
		using namespace Graphics;
		std::vector<Vertex> patchVertices(mNumPatchVertRows * mNumPatchVertCols);

		float halfWidth = 0.5f * GetWidth();
		float halfDepth = 0.5f * GetDepth();

		float patchWidth = GetWidth() / (mNumPatchVertCols - 1);
		float patchDepth = GetDepth() / (mNumPatchVertRows - 1);
		float du = 1.0f / (mNumPatchVertCols - 1);
		float dv = 1.0f / (mNumPatchVertRows - 1);

		for (UINT i = 0; i < mNumPatchVertRows; ++i)
		{
			float z = halfDepth - i * patchDepth;
			for (UINT j = 0; j < mNumPatchVertCols; ++j)
			{
				float x = -halfWidth + j * patchWidth;

				patchVertices[i * mNumPatchVertCols + j].Pos = XMFLOAT3(x, 0.0f, z);

				// Stretch texture over grid.
				patchVertices[i * mNumPatchVertCols + j].Tex.x = j * du;
				patchVertices[i * mNumPatchVertCols + j].Tex.y = i * dv;
			}
		}

		// Store axis-aligned bounding box y-bounds in upper-left patch corner.
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				UINT patchID = i * (mNumPatchVertCols - 1) + j;
				patchVertices[i * mNumPatchVertCols + j].BoundsY = mPatchBoundsY[patchID];
			}
		}

		m_pQuadPatchVB = Renderer::GetDevice()->CreateVertexBuffer(&patchVertices[0], sizeof(Vertex) * patchVertices.size());
	}
	void Terrain::BuildShaders()
	{
		using namespace Graphics;
		std::string vertexPath = "Shaders/Bin/TerrainShadowVS.cso";
		std::string hullPath = "Shaders/Bin/TerrainShadowHS.cso";
		std::string domainPath = "Shaders/Bin/TerrainShadowDS.cso";
		std::string pixelPath = "Shaders/Bin/TerrainShadowPS.cso";

		m_pVertexShader = Renderer::GetDevice()->CreateVertexShader(vertexPath);
		m_pHullShader = Renderer::GetDevice()->CreateHullShader(hullPath);
		m_pDomainShader = Renderer::GetDevice()->CreateDomainShader(domainPath);
		m_pPixelShader = Renderer::GetDevice()->CreatePixelShader(pixelPath);
	}
	void Terrain::BuildInputLayout(ID3D11Device* device)
	{
		//Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},		
			{"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},		
		};

		ID3DBlob* vsCode = (ID3DBlob*)m_pVertexShader->GetByteCode();

		WRECK_HR(device->CreateInputLayout(layoutDesc, std::size(layoutDesc), vsCode->GetBufferPointer(),
			vsCode->GetBufferSize(), &mInputLayout));
	}
	void Terrain::BuildTerrainCB()
	{
		using namespace Graphics;
		m_pTerrainCBHS = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(TerrainCBHS)));
		m_pTerrainCBDS = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(TerrainCBDS)));
		m_pTerrainCBPS = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(TerrainCBPS)));
		m_pShadowCB = Renderer::GetDevice()->CreateConstantBuffer(CalcConstantBufferByteSize(sizeof(ShadowCB)));
	}
	void Terrain::BuildQuadPatchIB()
	{
		using namespace Graphics;
		std::vector<unsigned int> indices(mNumPatchQuadFaces * 4); // 4 indices per quad face

	// Iterate over each quad and compute indices.
		int k = 0;
		for (UINT i = 0; i < mNumPatchVertRows - 1; ++i)
		{
			for (UINT j = 0; j < mNumPatchVertCols - 1; ++j)
			{
				// Top row of 2x2 quad patch
				indices[k] = i * mNumPatchVertCols + j;
				indices[k + 1] = i * mNumPatchVertCols + j + 1;

				// Bottom row of 2x2 quad patch
				indices[k + 2] = (i + 1) * mNumPatchVertCols + j;
				indices[k + 3] = (i + 1) * mNumPatchVertCols + j + 1;

				k += 4; // next quad
			}
		}

		m_pQuadPatchIB = Renderer::GetDevice()->CreateIndexBuffer(indices);
	}
	void Terrain::BuildHeightmapSRV(ID3D11Device* device)
	{
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = mInfo.HeightmapWidth;
		texDesc.Height = mInfo.HeightmapHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R16_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		// HALF is defined in xnamath.h, for storing 16-bit float.
		std::vector<PackedVector::HALF> hmap(mHeightmap.size());
		std::transform(mHeightmap.begin(), mHeightmap.end(), hmap.begin(), PackedVector::XMConvertFloatToHalf);

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = &hmap[0];
		data.SysMemPitch = mInfo.HeightmapWidth * sizeof(PackedVector::HALF);
		data.SysMemSlicePitch = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> hmapTex = 0;
		WRECK_HR(device->CreateTexture2D(&texDesc, &data, &hmapTex));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		WRECK_HR(device->CreateShaderResourceView(hmapTex.Get(), &srvDesc, &mHeightMapSRV));
	}
	Terrain::Terrain(unsigned entID)
		: mEntID(entID)
	{
		mInfo.HeightMapFilename = L"assets/Textures/terrain.raw";
		mInfo.LayerMapFilename0 = L"assets/Textures/grass.dds";
		mInfo.LayerMapFilename1 = L"assets/Textures/darkdirt.dds";
		mInfo.LayerMapFilename2 = L"assets/Textures/stone.dds";
		mInfo.LayerMapFilename3 = L"assets/Textures/lightdirt.dds";
		mInfo.LayerMapFilename4 = L"assets/Textures/snow.dds";
		mInfo.BlendMapFilename = L"assets/Textures/blend.dds";
		mInfo.HeightScale = 50.0f;
		mInfo.HeightmapWidth = 2049;
		mInfo.HeightmapHeight = 2049;
		mInfo.CellSpacing = 0.5f;

		ID3D11Device* device = reinterpret_cast<ID3D11Device*>(Graphics::Renderer::GetDevice()->GetNativePointer());
		ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(Graphics::Renderer::GetRenderContext()->GetNativePointer());

		// Divide heightmap into patches such that each patch has CellsPerPatch.
		mNumPatchVertRows = ((mInfo.HeightmapHeight - 1) / CellsPerPatch) + 1;
		mNumPatchVertCols = ((mInfo.HeightmapWidth - 1) / CellsPerPatch) + 1;

		mNumPatchVertices = mNumPatchVertRows * mNumPatchVertCols;
		mNumPatchQuadFaces = (mNumPatchVertRows - 1) * (mNumPatchVertCols - 1);
		LoadHeightMap();
		Smooth();
		CalcAllPatchBoundsY();

		BuildShaders();
		BuildInputLayout(device);
		BuildQuadPatchVB();
		BuildTerrainCB();
		BuildQuadPatchIB();
		BuildHeightmapSRV(device);
		std::vector<std::wstring> layerFilenames;
		layerFilenames.push_back(mInfo.LayerMapFilename0);
		layerFilenames.push_back(mInfo.LayerMapFilename1);
		layerFilenames.push_back(mInfo.LayerMapFilename2);
		layerFilenames.push_back(mInfo.LayerMapFilename3);
		layerFilenames.push_back(mInfo.LayerMapFilename4);
		//mLayerMapArraySRV = CreateTexture2DArraySRV(device, context,
		//	layerFilenames);

		{
			using namespace Graphics;
			SAMPLER_DESC samplerDesc = {};
			samplerDesc.AddressU = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_WRAP;
			samplerDesc.Filter = SAMPLE_FILTER::ANISOTROPIC;
			samplerDesc.MaxAnisotropy = 16;
			samplerDesc.MaxLOD = FLT_MAX;
			m_pSamplerState = Renderer::GetDevice()->CreateSamplerState(samplerDesc);
		}

		{
			using namespace Graphics;

			SAMPLER_DESC samplerDesc = {};

			samplerDesc.AddressU = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressW = TEXTURE_ADDRESS_MODE::TEXTURE_ADDRESS_BORDER;
			samplerDesc.Filter = SAMPLE_FILTER::MIN_MAG_MIP_POINT;

			samplerDesc.BorderColor[0] = 1.0f;
			samplerDesc.BorderColor[1] = 1.0f;
			samplerDesc.BorderColor[2] = 1.0f;
			samplerDesc.BorderColor[3] = 1.0f;

			samplerDesc.MaxLOD = FLT_MAX;

			m_pShadowSampler = Renderer::GetDevice()->CreateSamplerState(samplerDesc);
		}

		WRECK_HR(DirectX::CreateDDSTextureFromFile(device,
			context, mInfo.BlendMapFilename.c_str(), nullptr, &mBlendMapSRV));

		WRECK_HR(DirectX::CreateDDSTextureFromFile(device, context,
			L"assets/Textures/grass.dds", nullptr, &mLayerMapArraySRV));

		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.DepthClipEnable = TRUE;

		WRECK_HR(device->CreateRasterizerState(&rastDesc, &mWireframeRasterizer));
	}
}