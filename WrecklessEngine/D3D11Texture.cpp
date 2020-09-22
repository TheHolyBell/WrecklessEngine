#include "D3D11Texture.h"

namespace Graphics
{
	D3D11Texture::D3D11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
		: m_pView(srv)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
		m_pView->GetDesc(&_srvDesc);
		m_Format = (TEX_FORMAT)_srvDesc.Format;
		

		Microsoft::WRL::ComPtr<ID3D11Resource> _pResource;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTex;
		m_pView->GetResource(&_pResource);

		WRECK_HR(_pResource.As(&_pTex));

		D3D11_TEXTURE2D_DESC _texDesc = {};
		_pTex->GetDesc(&_texDesc);

		m_Width = _texDesc.Width;
		m_Height = _texDesc.Height;
	}
	TEX_FORMAT D3D11Texture::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11Texture::GetNativePointer() const
	{
		return m_pView.Get();
	}
	unsigned D3D11Texture::GetWidth() const
	{
		return m_Width;
	}
	unsigned D3D11Texture::GetHeight() const
	{
		return m_Height;
	}
	D3D11Texture::~D3D11Texture()
	{
		//IO::cout << "D3D11Texture() - " << m_pView.Reset() << IO::endl;
	}
	D3D11RenderTarget::D3D11RenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV)
		: m_RTV(std::move(RTV))
	{
		D3D11_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
		m_RTV->GetDesc(&_rtvDesc);
		m_Format = (TEX_FORMAT)_rtvDesc.Format;

		Microsoft::WRL::ComPtr<ID3D11Resource> _pResource;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTex;
		m_RTV->GetResource(&_pResource);

		WRECK_HR(_pResource.As(&_pTex));

		D3D11_TEXTURE2D_DESC _texDesc = {};
		_pTex->GetDesc(&_texDesc);

		m_Width = _texDesc.Width;
		m_Height = _texDesc.Height;
	}
	TEX_FORMAT D3D11RenderTarget::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11RenderTarget::GetNativePointer() const
	{
		return m_RTV.Get();
	}
	unsigned D3D11RenderTarget::GetWidth() const
	{
		return m_Width;
	}
	unsigned D3D11RenderTarget::GetHeight() const
	{
		return m_Height;
	}
	unsigned long D3D11RenderTarget::Reset()
	{
		return m_RTV.Reset();
	}
	D3D11RenderTarget::~D3D11RenderTarget()
	{
		//IO::cout << "D3D11RenderTarget() - " << m_RTV.Reset() << IO::endl;
	}
	D3D11DepthStencilView::D3D11DepthStencilView(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV)
		: m_DSV(DSV)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC _dssDesc = {};
		m_DSV->GetDesc(&_dssDesc);
		m_Format = (TEX_FORMAT)_dssDesc.Format;

		Microsoft::WRL::ComPtr<ID3D11Resource> _pResource;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _pTex;
		DSV->GetResource(&_pResource);

		WRECK_HR(_pResource.As(&_pTex));

		D3D11_TEXTURE2D_DESC _texDesc = {};
		_pTex->GetDesc(&_texDesc);

		m_Width = _texDesc.Width;
		m_Height = _texDesc.Height;
	}
	TEX_FORMAT D3D11DepthStencilView::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11DepthStencilView::GetNativePointer() const
	{
		return m_DSV.Get();
	}
	unsigned D3D11DepthStencilView::GetWidth() const
	{
		return m_Width;
	}
	unsigned D3D11DepthStencilView::GetHeight() const
	{
		return m_Height;
	}
	unsigned long D3D11DepthStencilView::Reset()
	{
		return m_DSV.Reset();
	}
	D3D11DepthStencilView::~D3D11DepthStencilView()
	{
		//IO::cout << "D3D11DepthStencilView() - " << m_DSV.Reset() << IO::endl;
	}
}