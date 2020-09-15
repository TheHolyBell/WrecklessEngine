#include "D3D11SwapChain.h"

#include "D3D11Texture.h"

#include "Renderer.h"
#include "Manipulators.h"

namespace Graphics
{
	D3D11SwapChain::D3D11SwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain)
		: m_pSwapChain(swap_chain)
	{
		ID3D11Device* _device = reinterpret_cast<ID3D11Device*>(Renderer::GetDevice()->GetNativePointer());
		ID3D11DeviceContext* _deviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());

		/*Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _rtv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _depthBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _dsv;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _backBufferPtr;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &_backBufferPtr);

		WRECK_HR(_device->CreateRenderTargetView(_backBufferPtr.Get(), nullptr, &_rtv));

		D3D11_TEXTURE2D_DESC _texDesc = {};

		_backBufferPtr->GetDesc(&_texDesc);

		D3D11_TEXTURE2D_DESC _depthBufferDesc = {};
		_depthBufferDesc.Width = _texDesc.Width;
		_depthBufferDesc.Height = _texDesc.Height;
		_depthBufferDesc.SampleDesc.Count = _texDesc.SampleDesc.Count;
		_depthBufferDesc.SampleDesc.Quality = _texDesc.SampleDesc.Quality;
		_depthBufferDesc.MipLevels = 1;
		_depthBufferDesc.ArraySize = 1;
		_depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		WRECK_HR(_device->CreateTexture2D(&_depthBufferDesc, nullptr, &_depthBuffer));
		WRECK_HR(_device->CreateDepthStencilView(_depthBuffer.Get(), nullptr, &_dsv));

		m_pRenderTarget = std::make_shared<D3D11RenderTarget>(_rtv);
		m_pDepthStencilView = std::make_shared<D3D11DepthStencilView>(_dsv);*/

		Microsoft::WRL::ComPtr<ID3D11Texture2D> swapChainBufferReference;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pTarget;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainBufferReference);
		_device->CreateRenderTargetView(swapChainBufferReference.Get(), nullptr, &_pTarget);

		m_pRenderTarget = std::make_shared<D3D11RenderTarget>(_pTarget);
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		m_pSwapChain->GetDesc(&swapChainDesc);

		D3D11_TEXTURE2D_DESC _depthDesc = {};
		_depthDesc.Width = swapChainDesc.BufferDesc.Width;
		_depthDesc.Height = swapChainDesc.BufferDesc.Height;
		_depthDesc.MipLevels = 1;
		_depthDesc.ArraySize = 1;
		_depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;


		_depthDesc.SampleDesc = swapChainDesc.SampleDesc;
		_depthDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pDepthBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
		WRECK_HR(_device->CreateTexture2D(&_depthDesc, nullptr, &_pDepthBuffer));
		WRECK_HR(_device->CreateDepthStencilView(_pDepthBuffer.Get(), nullptr, &_pDepthStencilView));

		m_pDepthStencilView = std::make_shared<D3D11DepthStencilView>(_pDepthStencilView);
	}
	void D3D11SwapChain::SwapBuffers(SwapFlags flag)
	{
		m_pSwapChain->Present((UINT)flag, 0);
	}
	void D3D11SwapChain::ResizeBuffers(UINT width, UINT height)
	{
		if (m_pSwapChain == nullptr)
			WRECK_ASSERT(false, "SwapChain is null");

		if (m_pRenderTarget == nullptr || m_pDepthStencilView == nullptr)
			WRECK_ASSERT(false, "SwapChain's buffers are not fully initialized");

		DXGI_FORMAT previousFormat = (DXGI_FORMAT)m_pRenderTarget->GetFormat();

		ID3D11Device* _pDevice = reinterpret_cast<ID3D11Device*>(Renderer::GetDevice()->GetNativePointer());

		m_pRenderTarget.reset();
		m_pDepthStencilView.reset();

		WRECK_HR(m_pSwapChain->ResizeBuffers(1, width, height, previousFormat, 0));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _backBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _pTarget;
		WRECK_HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backBuffer.GetAddressOf())));
		WRECK_HR(_pDevice->CreateRenderTargetView(_backBuffer.Get(), nullptr, &_pTarget));

		m_pRenderTarget = std::make_shared<D3D11RenderTarget>(std::move(_pTarget));
		
		D3D11_TEXTURE2D_DESC _depthDesc = {};
		_depthDesc.Width = width;
		_depthDesc.Height = height;
		_depthDesc.MipLevels = 1;
		_depthDesc.ArraySize = 1;
		_depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		WRECK_HR(m_pSwapChain->GetDesc(&swapChainDesc));

		_depthDesc.SampleDesc = swapChainDesc.SampleDesc;
		_depthDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pDepthBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
		WRECK_HR(_pDevice->CreateTexture2D(&_depthDesc, nullptr, &_pDepthBuffer));
		WRECK_HR(_pDevice->CreateDepthStencilView(_pDepthBuffer.Get(), nullptr, &_pDepthStencilView));
		_pDepthBuffer.Reset();
		m_pDepthStencilView = std::make_shared<D3D11DepthStencilView>(_pDepthStencilView);
	}
	Ref<IRenderTarget> D3D11SwapChain::GetBackBuffer() const
	{
		return m_pRenderTarget;
	}
	Ref<IDepthStencilView> D3D11SwapChain::GetDepthStencilView() const
	{
		return m_pDepthStencilView;
	}
	void* D3D11SwapChain::GetNativePointer() const
	{
		return m_pSwapChain.Get();
	}
}