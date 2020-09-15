#pragma once

#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"
#include "IBuffer.h"
#include "Vertex.h"
#include "IInputLayout.h"
#include "DynamicConstant.h"
#include "TextureDesc.h"

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

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) PURE;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) PURE;

		virtual Ref<IVertexBuffer> CreateVertexBuffer(const Dynamic::VertexBuffer& buffer) PURE;
		virtual Ref<IIndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& buffer) PURE;

		virtual Ref<IConstantBuffer> CreateConstantBuffer(unsigned int size, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const void* data, unsigned size, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::LayoutElement& layout_root, const Dynamic::Buffer* buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		
		virtual Ref<IInputLayout> CreateInputLayout(const Dynamic::VertexLayout& layout, void* compiled_shader) PURE;
	};
}