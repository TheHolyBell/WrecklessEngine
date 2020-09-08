#include "D3D11Device.h"

#include "D3D11Texture.h"

#include <d3dcompiler.h>

#include "FileHelper.h"
#include "StringHelper.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "Renderer.h"

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
	Ref<ITexture> D3D11Device::CreateTexture(const std::string& path)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;
		ID3D11DeviceContext* _deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());
		
		auto file_extension = FileSystem::FileHelper::GetFileExtension(path);

		if (file_extension == ".dds")
			DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), _deviceContext, Misc::StringHelper::ToWide(path).c_str(), nullptr, &_srv);
		else
			DirectX::CreateWICTextureFromFile(m_pDevice.Get(), _deviceContext, Misc::StringHelper::ToWide(path).c_str(), nullptr, &_srv);

		return std::make_shared<D3D11Texture>(_srv);
	}
	Ref<ITexture> D3D11Device::CreateTexture(UINT width, UINT height)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSrv;
		WRECK_ASSERT(false, "Not yet implemented");
		return Ref<ITexture>(nullptr);
	}
	Ref<IVertexShader> D3D11Device::CreateVertexShader(const std::string& path)
	{
		return Ref<IVertexShader>();
	}
	Ref<IPixelShader> D3D11Device::CreatePixelShader(const std::string& path)
	{
		return Ref<IPixelShader>();
	}
}