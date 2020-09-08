#pragma once

#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"

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
	};
}