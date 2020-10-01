#pragma once

#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"
#include "IBuffer.h"
#include "Vertex.h"
#include "IInputLayout.h"
#include "IRasterizer.h"
#include "DynamicConstant.h"
#include "TextureDesc.h"
#include "ISamplerState.h"
#include "IDepthStencilState.h"

namespace Graphics
{
	class IDevice
	{
	public:
		IDevice() = default;

		IDevice(const IDevice& rhs) = delete;
		IDevice& operator=(const IDevice& rhs) = delete;

		IDevice(IDevice&& rhs) = delete;
		IDevice& operator=(IDevice&& rhs) = delete;

		virtual ~IDevice() = default;

		virtual void* GetNativePointer() const PURE;


		virtual Ref<ITexture> CreateTexture2D(const std::string& path) PURE;
		virtual Ref<ITexture> CreateTexture2D(TEXTURE2D_DESC texture_desc) PURE;

		virtual Ref<ITexture> CreateTexture3D(const std::string& path) PURE;

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) PURE;
		virtual Ref<IHullShader> CreateHullShader(const std::string& path) PURE;
		virtual Ref<IDomainShader> CreateDomainShader(const std::string& path) PURE;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) PURE;

		virtual Ref<IRasterizer> CreateRasterizer(RASTERIZER_DESC rasterizer_desc) PURE;
		virtual Ref<ISamplerState> CreateSamplerState(SAMPLER_DESC sampler_desc) PURE;
		virtual Ref<IDepthStencilState> CreateDepthStencilState(DEPTH_STENCIL_DESC depth_stencil_desc) PURE;

		virtual Ref<IVertexBuffer> CreateVertexBuffer(const Dynamic::VertexBuffer& buffer) PURE;
		virtual Ref<IVertexBuffer> CreateVertexBuffer(const void* data, unsigned size_in_bytes) PURE;
		virtual Ref<IIndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& buffer) PURE;

		virtual Ref<IConstantBuffer> CreateConstantBuffer(unsigned int size, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const void* data, unsigned size, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::LayoutElement& layout_root, const Dynamic::Buffer* buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		
		virtual Ref<IInputLayout> CreateInputLayout(const Dynamic::VertexLayout& layout, void* compiled_shader) PURE;
	};
}