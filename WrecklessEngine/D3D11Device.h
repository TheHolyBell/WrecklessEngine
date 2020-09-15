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


		virtual Ref<ITexture> CreateTexture2D(const std::string& path) override;
		virtual Ref<ITexture> CreateTexture2D(TEXTURE2D_DESC texture_desc) override;

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) override;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) override;

		virtual Ref<IVertexBuffer> CreateVertexBuffer(const Dynamic::VertexBuffer& buffer) override;
		virtual Ref<IIndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& buffer) override;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) override;
		virtual Ref<IInputLayout> CreateInputLayout(const Dynamic::VertexLayout& layout, Ref<IVertexShader> vertex_shader) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	};
}