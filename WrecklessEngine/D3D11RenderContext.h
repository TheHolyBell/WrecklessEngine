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

		virtual void SetOutputTarget(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil) override;
		virtual void SetOutputRenderTarget(Ref<IRenderTarget> render_target) override;
		virtual void SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil) override;

		virtual void ClearRenderTarget(Ref<IRenderTarget> render_target, float* color) override;
		virtual void ClearDepthStencilView(Ref<IDepthStencilView> depth_stencil, float depth, UINT stencil = 0) override;

		virtual void BindVertexShader(Ref<IVertexShader> vertex_shader) override;
		virtual void BindHullShader(Ref<IHullShader> hull_shader) override;
		virtual void BindDomainShader(Ref<IDomainShader> domain_shader) override;
		virtual void BindPixelShader(Ref<IPixelShader> pixel_shader) override;
		virtual void BindViewport(Viewport viewport) override;

		virtual void BindVertexBuffer(Ref<IVertexBuffer> vertex_buffer, unsigned strides, unsigned offsets) override;
		virtual void BindIndexBuffer(Ref<IIndexBuffer> index_buffer,  unsigned offset) override;
		virtual void BindConstantBuffer(Ref<IConstantBuffer> constant_buffer, SHADER_TYPE stage, int slot) override;
		virtual void BindInputLayout(Ref<IInputLayout> input_layout) override;
		virtual void BindSamplerState(Ref<ISamplerState> sampler_state, unsigned slot) override;
		virtual void BindRasterizerState(Ref<IRasterizer> rasterizer_state) override;
		virtual void BindDepthStencilState(Ref<IDepthStencilState> depth_stencil_state) override;
		virtual void BindTopology(PRIMITIVE_TOPOLOGY topology) override;
		virtual void BindTexture2D(Ref<ITexture> texture, SHADER_TYPE stage, unsigned slot) override;

		virtual void MapDataToBuffer(Ref<IBuffer> buffer, const void* data, unsigned size) override;
		virtual void LoadDataFromBuffer(Ref<IBuffer> buffer, void* outputBuffer, unsigned size) override;

		virtual void Draw(unsigned vertex_count, unsigned start_vertex_location) override;
		virtual void DrawIndexed(unsigned index_count, unsigned start_index_location, unsigned base_vertex_location) override;

		virtual void* GetNativePointer() const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	};
}