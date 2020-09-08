#include "D3D11Texture.h"

namespace Graphics
{
	D3D11Texture::D3D11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
		: m_pView(srv)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
		m_pView->GetDesc(&_srvDesc);
		m_Format = (TEX_FORMAT)_srvDesc.Format;
	}
	TEX_FORMAT D3D11Texture::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11Texture::GetNativePointer() const
	{
		return m_pView.Get();
	}
	D3D11RenderTarget::D3D11RenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV)
		: m_RTV(RTV)
	{
		D3D11_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
		m_RTV->GetDesc(&_rtvDesc);
		m_Format = (TEX_FORMAT)_rtvDesc.Format;
	}
	TEX_FORMAT D3D11RenderTarget::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11RenderTarget::GetNativePointer() const
	{
		return m_RTV.Get();
	}
	D3D11DepthStencilView::D3D11DepthStencilView(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV)
		: m_DSV(DSV)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC _dssDesc = {};
		m_DSV->GetDesc(&_dssDesc);
		m_Format = (TEX_FORMAT)_dssDesc.Format;
	}
	TEX_FORMAT D3D11DepthStencilView::GetFormat() const
	{
		return m_Format;
	}
	void* D3D11DepthStencilView::GetNativePointer() const
	{
		return m_DSV.Get();
	}
}