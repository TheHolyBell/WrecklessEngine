#pragma once
#include "CommonInclude.h"
#include "ISwapChain.h"
#include "D3D11Texture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11SwapChain : public ISwapChain
	{
	public:
		D3D11SwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain);

		virtual void SwapBuffers(SwapFlags flag = SwapFlags::NO_LIMIT) override;

		virtual void ResizeBuffers(UINT width, UINT height) override;

		virtual Ref<IRenderTarget> GetBackBuffer() const override;
		virtual Ref<IDepthStencilView> GetDepthStencilView() const override;

		virtual void* GetNativePointer() const override;
	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Ref<D3D11RenderTarget> m_pRenderTarget;
		Ref<D3D11DepthStencilView> m_pDepthStencilView;
	};
}