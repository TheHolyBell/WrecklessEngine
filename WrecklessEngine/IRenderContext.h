#pragma once
#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"
#include "Viewport.h"

namespace Graphics
{
	class IRenderContext
	{
	public:
		IRenderContext() = default;

		IRenderContext(const IRenderContext& rhs) = delete;
		IRenderContext& operator=(const IRenderContext& rhs) = delete;

		IRenderContext(IRenderContext&& rhs) = delete;
		IRenderContext& operator=(IRenderContext&& rhs) = delete;

		virtual ~IRenderContext() = default;

		void SetOutputTargets(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil);
		virtual void SetOutputRenderTarget(Ref<IRenderTarget> render_target) PURE;
		virtual void SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil) PURE;

		virtual void ClearRenderTarget(Ref<IRenderTarget> render_target, float* color) PURE;
		virtual void ClearDepthStencilView(Ref<IDepthStencilView> depth_stencil, float depth, UINT stencil = 0) PURE;

		virtual void BindVertexShader(Ref<IVertexShader> vertex_shader) PURE;
		virtual void BindPixelShader(Ref<IPixelShader> pixel_shader) PURE;
		virtual void BindViewport(Viewport viewport) PURE;

		virtual void* GetNativePointer() const PURE;
		// ----------------------------------
	};
}