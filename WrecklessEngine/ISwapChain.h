#pragma once
#include "CommonInclude.h"
#include "ITexture.h"

namespace Graphics
{
	enum class SwapFlags
	{
		NO_LIMIT = 0,
		RATE_60 = 1,
		RATE_30 = 2
	};

	class ISwapChain
	{
	public:
		ISwapChain() = default;

		ISwapChain(const ISwapChain& rhs) = delete;
		ISwapChain& operator=(const ISwapChain& rhs) = delete;

		ISwapChain(ISwapChain&& rhs) = delete;
		ISwapChain& operator=(ISwapChain&& rhs) = delete;

		virtual ~ISwapChain() = default;

		virtual void SwapBuffers(SwapFlags flag = SwapFlags::NO_LIMIT) PURE;
		
		virtual void ResizeBuffers(UINT width, UINT height) PURE;

		virtual Ref<IRenderTarget> GetBackBuffer() const PURE;
		virtual Ref<IDepthStencilView> GetDepthStencilView() const PURE;

		virtual void* GetNativePointer() const PURE;
	};
}