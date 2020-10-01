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

		virtual Ref<ITexture> CreateTexture3D(const std::string& path) override;

		virtual Ref<IRasterizer> CreateRasterizer(RASTERIZER_DESC rasterizer_desc) override;
		virtual Ref<ISamplerState> CreateSamplerState(SAMPLER_DESC sampler_desc) override;
		virtual Ref<IDepthStencilState> CreateDepthStencilState(DEPTH_STENCIL_DESC depth_stencil_desc) override;

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) override;
		virtual Ref<IHullShader> CreateHullShader(const std::string& path) override;
		virtual Ref<IDomainShader> CreateDomainShader(const std::string& path) override;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) override;

		virtual Ref<IVertexBuffer> CreateVertexBuffer(const Dynamic::VertexBuffer& buffer) override;
		virtual Ref<IVertexBuffer> CreateVertexBuffer(const void* data, unsigned size_in_bytes) override;
		virtual Ref<IIndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& buffer) override;
		
		virtual Ref<IConstantBuffer> CreateConstantBuffer(unsigned int size, BUFFER_USAGE usage = USAGE_DYNAMIC) override;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) override;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const void* data, unsigned size, BUFFER_USAGE usage = USAGE_DYNAMIC) override;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::LayoutElement& layout_root, const Dynamic::Buffer* buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) override;
		
		virtual Ref<IInputLayout> CreateInputLayout(const Dynamic::VertexLayout& layout, void* compiled_shader) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	};
}