#pragma once
#include "CommonInclude.h"
#include "ITexture.h"
#include "IShader.h"
#include "Viewport.h"
#include "IBuffer.h"
#include "IInputLayout.h"
#include "TopologyTypes.h"
#include "ISamplerState.h"
#include "IRasterizer.h"
#include "IDepthStencilState.h"

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

		virtual void SetOutputTarget(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil) PURE;
		virtual void SetOutputRenderTarget(Ref<IRenderTarget> render_target) PURE;
		virtual void SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil) PURE;

		virtual void ClearRenderTarget(Ref<IRenderTarget> render_target, float* color) PURE;
		virtual void ClearDepthStencilView(Ref<IDepthStencilView> depth_stencil, float depth, UINT stencil = 0) PURE;

		virtual void BindVertexShader(Ref<IVertexShader> vertex_shader) PURE;
		virtual void BindHullShader(Ref<IHullShader> hull_shader) PURE;
		virtual void BindDomainShader(Ref<IDomainShader> domain_shader) PURE;
		virtual void BindPixelShader(Ref<IPixelShader> pixel_shader) PURE;
		virtual void BindViewport(Viewport viewport) PURE;

		virtual void BindVertexBuffer(Ref<IVertexBuffer> vertex_buffer, unsigned strides, unsigned offsets) PURE;
		virtual void BindIndexBuffer(Ref<IIndexBuffer> index_buffer, unsigned offset) PURE;
		virtual void BindConstantBuffer(Ref<IConstantBuffer> constant_buffer, SHADER_TYPE stage, int slot) PURE;
		virtual void BindInputLayout(Ref<IInputLayout> input_layout) PURE;
		virtual void BindSamplerState(Ref<ISamplerState> sampler_state, unsigned slot) PURE;
		virtual void BindRasterizerState(Ref<IRasterizer> rasterizer_state) PURE;
		virtual void BindDepthStencilState(Ref<IDepthStencilState> depth_stencil_state) PURE;
		virtual void BindTopology(PRIMITIVE_TOPOLOGY topology) PURE;
		virtual void BindTexture2D(Ref<ITexture> texture, SHADER_TYPE stage, unsigned slot) PURE;

		virtual void MapDataToBuffer(Ref<IBuffer> buffer, const void* data, unsigned size) PURE;
		virtual void LoadDataFromBuffer(Ref<IBuffer> buffer, void* outputBuffer, unsigned size) PURE;

		virtual void Draw(unsigned vertex_count, unsigned start_vertex_location) PURE;
		virtual void DrawIndexed(unsigned index_count, unsigned start_index_location, unsigned base_vertex_location) PURE;

		virtual void* GetNativePointer() const PURE;
		// ----------------------------------
	};
}