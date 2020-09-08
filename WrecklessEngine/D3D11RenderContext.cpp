#include "D3D11RenderContext.h"

namespace Graphics
{
	D3D11RenderContext::D3D11RenderContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
		: m_pDeviceContext(pDeviceContext)
	{
	}
	void D3D11RenderContext::SetOutputRenderTarget(Ref<IRenderTarget> render_target)
	{
		ID3D11RenderTargetView* _rtv = reinterpret_cast<ID3D11RenderTargetView*>(render_target->GetNativePointer());
		m_pDeviceContext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	void D3D11RenderContext::SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil)
	{
		std::vector<ID3D11RenderTargetView*> rtvs;

		for(auto& rt : render_targets)
			rtvs.push_back(reinterpret_cast<ID3D11RenderTargetView*>(rt->GetNativePointer()));
		m_pDeviceContext->OMSetRenderTargets(rtvs.size(), rtvs.data(), reinterpret_cast<ID3D11DepthStencilView*>(depth_stencil->GetNativePointer()));
	}
	void D3D11RenderContext::ClearRenderTarget(Ref<IRenderTarget> render_target, float* color)
	{
		m_pDeviceContext->ClearRenderTargetView(reinterpret_cast<ID3D11RenderTargetView*>(render_target->GetNativePointer()), color);
	}
	void D3D11RenderContext::ClearDepthStencilView(Ref<IDepthStencilView> depth_stencil, float depth, UINT stencil)
	{
		m_pDeviceContext->ClearDepthStencilView(reinterpret_cast<ID3D11DepthStencilView*>(depth_stencil->GetNativePointer()), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	}
	void D3D11RenderContext::BindVertexShader(Ref<IVertexShader> vertex_shader)
	{
		m_pDeviceContext->VSSetShader(reinterpret_cast<ID3D11VertexShader*>(vertex_shader->GetNativePointer()), nullptr, 0);
	}
	void D3D11RenderContext::BindPixelShader(Ref<IPixelShader> pixel_shader)
	{
		m_pDeviceContext->PSSetShader(reinterpret_cast<ID3D11PixelShader*>(pixel_shader->GetNativePointer()), nullptr, 0);
	}
	void D3D11RenderContext::BindViewport(Viewport viewport)
	{
		D3D11_VIEWPORT vp = {};
		vp.Width = viewport.Width;
		vp.Height = viewport.Height;
		vp.MinDepth = viewport.MinDepth;
		vp.MaxDepth = viewport.MaxDepth;
		vp.TopLeftX = viewport.TopLeftX;
		vp.TopLeftY = viewport.TopLeftY;

		m_pDeviceContext->RSSetViewports(1, &vp);
	}
	void* D3D11RenderContext::GetNativePointer() const
	{
		return m_pDeviceContext.Get();
	}
}