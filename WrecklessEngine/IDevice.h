#pragma once

#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"
#include "IBuffer.h"
#include "Vertex.h"
#include "IInputLayout.h"
#include "DynamicConstant.h"

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


		virtual Ref<ITexture> CreateTexture(const std::string& path) PURE;
		virtual Ref<ITexture> CreateTexture(UINT width, UINT height) PURE;

		virtual Ref<IVertexShader> CreateVertexShader(const std::string& path) PURE;
		virtual Ref<IPixelShader> CreatePixelShader(const std::string& path) PURE;

		virtual Ref<IVertexBuffer> CreateVertexBuffer(const Dynamic::VertexBuffer& buffer) PURE;
		virtual Ref<IIndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& buffer) PURE;
		virtual Ref<IConstantBuffer> CreateConstantBuffer(const Dynamic::Buffer& buffer, BUFFER_USAGE usage = USAGE_DYNAMIC) PURE;
		virtual Ref<IInputLayout> CreateInputLayout(const Dynamic::VertexLayout& layout, Ref<IVertexShader> vertex_shader) PURE;
	};
}