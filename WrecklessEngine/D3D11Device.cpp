#include "D3D11Device.h"

#include "D3D11Texture.h"

#include <d3dcompiler.h>

#include "FileHelper.h"
#include "StringHelper.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "Renderer.h"

#include "D3D11Buffer.h"
#include "D3D11InputLayout.h"
#include "D3D11Shader.h"
#include "D3D11Rasterizer.h"
#include "D3D11SamplerState.h"
#include "D3D11DepthStencilState.h"

namespace Graphics
{
	D3D11Device::D3D11Device(Microsoft::WRL::ComPtr<ID3D11Device> pDevice)
		: m_pDevice(pDevice)
	{
	}
	void* D3D11Device::GetNativePointer() const
	{
		return m_pDevice.Get();
	}
	Ref<ITexture> D3D11Device::CreateTexture2D(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;
		ID3D11DeviceContext* _deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());
		
		auto file_extension = FileSystem::FileHelper::GetFileExtension(path);

		if (file_extension == ".dds")
		{
			WRECK_HR(DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), _deviceContext, Misc::StringHelper::ToWide(path).c_str(), nullptr, &_srv));
		}
		else
		{
			WRECK_HR(DirectX::CreateWICTextureFromFile(m_pDevice.Get(), _deviceContext, Misc::StringHelper::ToWide(path).c_str(), nullptr, &_srv));
		}
		
		return std::make_shared<D3D11Texture>(_srv);
	}
	Ref<ITexture> D3D11Device::CreateTexture2D(TEXTURE2D_DESC texture_desc)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSrv;

		WRECK_HR(m_pDevice->CreateTexture2D(reinterpret_cast<D3D11_TEXTURE2D_DESC*>(&texture_desc), nullptr, &_pTexture));
		WRECK_HR(m_pDevice->CreateShaderResourceView(_pTexture.Get(), nullptr, &_pSrv));
		
		return std::make_shared<D3D11Texture>(_pSrv);
	}
	Ref<ITexture> D3D11Device::CreateTexture3D(const std::string& path)
	{
		return CreateTexture2D(path);
	}
	Ref<IRasterizer> D3D11Device::CreateRasterizer(RASTERIZER_DESC rasterizer_desc)
	{
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _pRasterizer;

		WRECK_HR(m_pDevice->CreateRasterizerState(reinterpret_cast<D3D11_RASTERIZER_DESC*>(&rasterizer_desc), &_pRasterizer));

		return std::make_shared<D3D11Rasterizer>(_pRasterizer);
	}
	Ref<ISamplerState> D3D11Device::CreateSamplerState(SAMPLER_DESC sampler_desc)
	{
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _pSamplerState;

		WRECK_HR(m_pDevice->CreateSamplerState(reinterpret_cast<D3D11_SAMPLER_DESC*>(&sampler_desc), &_pSamplerState));

		return std::make_shared<D3D11SamplerState>(_pSamplerState);
	}
	Ref<IDepthStencilState> D3D11Device::CreateDepthStencilState(DEPTH_STENCIL_DESC depth_stencil_desc)
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _pDepthStencilState;

		WRECK_HR(m_pDevice->CreateDepthStencilState(reinterpret_cast<D3D11_DEPTH_STENCIL_DESC*>(&depth_stencil_desc), &_pDepthStencilState));

		return std::make_shared<D3D11DepthStencilState>(_pDepthStencilState);
	}
	Ref<IVertexShader> D3D11Device::CreateVertexShader(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> _pCompiledCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _pVertexShader;

		WRECK_HR(D3DReadFileToBlob(Misc::StringHelper::ToWide(path).c_str(), &_pCompiledCode));
		WRECK_HR(m_pDevice->CreateVertexShader(_pCompiledCode->GetBufferPointer(), _pCompiledCode->GetBufferSize(), nullptr, &_pVertexShader));

		return std::make_shared<D3D11VertexShader>(_pVertexShader, _pCompiledCode);
	}
	Ref<IHullShader> D3D11Device::CreateHullShader(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> _pCompiledCode;
		Microsoft::WRL::ComPtr<ID3D11HullShader> _pHullShader;

		WRECK_HR(D3DReadFileToBlob(Misc::StringHelper::ToWide(path).c_str(), &_pCompiledCode));
		WRECK_HR(m_pDevice->CreateHullShader(_pCompiledCode->GetBufferPointer(), _pCompiledCode->GetBufferSize(), nullptr, &_pHullShader));

		return std::make_shared<D3D11HullShader>(_pHullShader, _pCompiledCode);
	}
	Ref<IDomainShader> D3D11Device::CreateDomainShader(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> _pCompiledCode;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> _pDomainShader;

		WRECK_HR(D3DReadFileToBlob(Misc::StringHelper::ToWide(path).c_str(), &_pCompiledCode));
		WRECK_HR(m_pDevice->CreateDomainShader(_pCompiledCode->GetBufferPointer(), _pCompiledCode->GetBufferSize(), nullptr, &_pDomainShader));

		return std::make_shared<D3D11DomainShader>(_pDomainShader, _pCompiledCode);
	}
	Ref<IPixelShader> D3D11Device::CreatePixelShader(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> _pCompiledCode;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> _pPixelShader;

		WRECK_HR(D3DReadFileToBlob(Misc::StringHelper::ToWide(path).c_str(), &_pCompiledCode));
		WRECK_HR(m_pDevice->CreatePixelShader(_pCompiledCode->GetBufferPointer(), _pCompiledCode->GetBufferSize(), nullptr, &_pPixelShader));

		return std::make_shared<D3D11PixelShader>(_pPixelShader, _pCompiledCode);
	}
	Ref<IVertexBuffer> D3D11Device::CreateVertexBuffer(const Dynamic::VertexBuffer& buffer)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _vbDesc = {};

		_vbDesc.ByteWidth = buffer.SizeBytes();
		_vbDesc.Usage = D3D11_USAGE_DEFAULT;
		_vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA _vinitData = {};
		_vinitData.pSysMem = buffer.GetData();

		WRECK_HR(m_pDevice->CreateBuffer(&_vbDesc, &_vinitData, &_pBuffer));

		return std::make_shared<D3D11VertexBuffer>(_pBuffer);
	}
	Ref<IVertexBuffer> D3D11Device::CreateVertexBuffer(const void* data, unsigned size_in_bytes)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _vbDesc = {};
		_vbDesc.ByteWidth = size_in_bytes;
		_vbDesc.Usage = D3D11_USAGE_DEFAULT;
		_vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA _vinitData = {};
		_vinitData.pSysMem = data;

		WRECK_HR(m_pDevice->CreateBuffer(&_vbDesc, &_vinitData, &_pBuffer));

		return std::make_shared<D3D11VertexBuffer>(_pBuffer);
	}
	Ref<IIndexBuffer> D3D11Device::CreateIndexBuffer(const std::vector<unsigned int>& buffer)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _ibDesc = {};

		_ibDesc.ByteWidth = buffer.size() * sizeof(unsigned int);
		_ibDesc.Usage = D3D11_USAGE_DEFAULT;
		_ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA _iinitData = {};
		_iinitData.pSysMem = buffer.data();

		WRECK_HR(m_pDevice->CreateBuffer(&_ibDesc, &_iinitData, &_pBuffer));

		return std::make_shared<D3D11IndexBuffer>(_pBuffer);
	}
	Ref<IConstantBuffer> D3D11Device::CreateConstantBuffer(unsigned int size, BUFFER_USAGE usage)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _cbDesc = {};
		_cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		_cbDesc.ByteWidth = size;
		_cbDesc.Usage = (D3D11_USAGE)usage;
		
		switch (usage)
		{
		case USAGE_DYNAMIC:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case USAGE_STAGING:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			break;
		}

		WRECK_HR(m_pDevice->CreateBuffer(&_cbDesc, nullptr, &_pBuffer));

		return std::make_shared<D3D11ConstantBuffer>(_pBuffer);
	}
	Ref<IConstantBuffer> D3D11Device::CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;
		
		D3D11_BUFFER_DESC _cbDesc = {};

		_cbDesc.ByteWidth = buffer.GetSizeInBytes();
		_cbDesc.Usage = (D3D11_USAGE)usage;
		_cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		
		switch (usage)
		{
		case USAGE_DYNAMIC:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case USAGE_STAGING:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			break;
		}

		D3D11_SUBRESOURCE_DATA _cbInitData = {};
		_cbInitData.pSysMem = buffer.GetData();

		WRECK_HR(m_pDevice->CreateBuffer(&_cbDesc, &_cbInitData, &_pBuffer));

		return std::make_shared<D3D11ConstantBuffer>(_pBuffer);
	}
	Ref<IConstantBuffer> D3D11Device::CreateConstantBuffer(const void* data, unsigned size, BUFFER_USAGE usage)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _cbDesc = {};
		_cbDesc.ByteWidth = size;
		_cbDesc.Usage = (D3D11_USAGE)usage;
		_cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		switch (usage)
		{
		case USAGE_DYNAMIC:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case USAGE_STAGING:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			break;
		}

		D3D11_SUBRESOURCE_DATA _cbInit = {};
		_cbInit.pSysMem = data;
		WRECK_HR(m_pDevice->CreateBuffer(&_cbDesc, &_cbInit, &_pBuffer));

		return std::make_shared<D3D11ConstantBuffer>(_pBuffer);
	}
	Ref<IConstantBuffer> D3D11Device::CreateConstantBuffer(const Dynamic::LayoutElement& layout_root, const Dynamic::Buffer* buffer, BUFFER_USAGE usage)
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> _pBuffer;

		D3D11_BUFFER_DESC _cbDesc = {};

		_cbDesc.ByteWidth = layout_root.GetSizeInBytes();
		_cbDesc.Usage = (D3D11_USAGE)usage;
		_cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		switch (usage)
		{
		case USAGE_DYNAMIC:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;
		case USAGE_STAGING:
			_cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			break;
		}

		if (buffer != nullptr)
		{
			D3D11_SUBRESOURCE_DATA _initData = {};
			_initData.pSysMem = buffer->GetData();
			WRECK_HR(m_pDevice->CreateBuffer(&_cbDesc, &_initData, &_pBuffer));
		}
		else
		{
			WRECK_HR(m_pDevice->CreateBuffer(&_cbDesc, nullptr, &_pBuffer));
		}

		return std::make_shared<D3D11ConstantBuffer>(_pBuffer);
	}
	Ref<IInputLayout> D3D11Device::CreateInputLayout(const Dynamic::VertexLayout& layout, void* compiled_shader)
	{
		Microsoft::WRL::ComPtr<ID3D11InputLayout> _pInputLayout;

		auto layoutDesc = layout.GetD3DLayout();
		ID3DBlob* pCode = reinterpret_cast<ID3DBlob*>(compiled_shader);

		WRECK_HR(m_pDevice->CreateInputLayout(layoutDesc.data(), layoutDesc.size(), pCode->GetBufferPointer(), pCode->GetBufferSize(), &_pInputLayout));

		return std::make_shared<D3D11InputLayout>(_pInputLayout);
	}
}