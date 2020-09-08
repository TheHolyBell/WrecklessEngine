#pragma once

#include "CommonInclude.h"
#include "IRenderContext.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11RenderContext : public IRenderContext
	{
	public:
		D3D11RenderContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext);

		virtual void SetOutputRenderTarget(Ref<IRenderTarget> render_target) override;
		virtual void SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil) override;

		virtual void ClearRenderTarget(Ref<IRenderTarget> render_target, float* color) override;
		virtual void ClearDepthStencilView(Ref<IDepthStencilView> depth_stencil, float depth, UINT stencil = 0) override;

		virtual void BindVertexShader(Ref<IVertexShader> vertex_shader) override;
		virtual void BindPixelShader(Ref<IPixelShader> pixel_shader) override;
		virtual void BindViewport(Viewport viewport) override;

		virtual void* GetNativePointer() const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	};
}