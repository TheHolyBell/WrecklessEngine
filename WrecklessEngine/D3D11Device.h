#pragma once

#include "CommonInclude.h"
#include "IDevice.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11Device : public IDevice
	{
	public:
		D3D11Device(Microsoft::WRL::ComPtr<ID3D11Device> pDevice);

		virtual void* GetNativePointer() const override;


		virtual Ref<ITexture> CreateTexture(const std::string& path) override;
		virtual Ref<ITexture> CreateTexture(UINT width, UINT height) override;

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) override;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	};
}