#include "D3D11RenderContext.h"

namespace Graphics
{
	D3D11RenderContext::D3D11RenderContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext)
		: m_pDeviceContext(pDeviceContext)
	{
	}
	void D3D11RenderContext::SetOutputTarget(Ref<IRenderTarget> render_target, Ref<IDepthStencilView> depth_stencil)
	{
		ID3D11RenderTargetView* rtv = nullptr;
		if (render_target != nullptr)
		{
			rtv = reinterpret_cast<ID3D11RenderTargetView*>(render_target->GetNativePointer());
		}
		m_pDeviceContext->OMSetRenderTargets(1, &rtv, reinterpret_cast<ID3D11DepthStencilView*>(depth_stencil->GetNativePointer()));
	}
	void D3D11RenderContext::SetOutputRenderTarget(Ref<IRenderTarget> render_target)
	{
		ID3D11RenderTargetView* _rtv = reinterpret_cast<ID3D11RenderTargetView*>(render_target->GetNativePointer());
		m_pDeviceContext->OMSetRenderTargets(1, &_rtv, nullptr);
	}
	void D3D11RenderContext::SetOutputTargets(std::vector<Ref<IRenderTarget>> render_targets, Ref<IDepthStencilView> depth_stencil)
	{
		std::vector<ID3D11RenderTargetView*> rtvs;

		for (auto& rt : render_targets)
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
		if (vertex_shader != nullptr)
			m_pDeviceContext->VSSetShader(reinterpret_cast<ID3D11VertexShader*>(vertex_shader->GetNativePointer()), nullptr, 0);
		else
			m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
	}
	void D3D11RenderContext::BindHullShader(Ref<IHullShader> hull_shader)
	{
		if(hull_shader != nullptr)
			m_pDeviceContext->HSSetShader(reinterpret_cast<ID3D11HullShader*>(hull_shader->GetNativePointer()), nullptr, 0);
		else
			m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	}
	void D3D11RenderContext::BindDomainShader(Ref<IDomainShader> domain_shader)
	{
		if(domain_shader != nullptr)
			m_pDeviceContext->DSSetShader(reinterpret_cast<ID3D11DomainShader*>(domain_shader->GetNativePointer()), nullptr, 0);
		else
			m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	}
	void D3D11RenderContext::BindPixelShader(Ref<IPixelShader> pixel_shader)
	{
		if(pixel_shader != nullptr)
			m_pDeviceContext->PSSetShader(reinterpret_cast<ID3D11PixelShader*>(pixel_shader->GetNativePointer()), nullptr, 0);
		else
			m_pDeviceContext->PSSetShader(nullptr, nullptr, 0);
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
	void D3D11RenderContext::BindVertexBuffer(Ref<IVertexBuffer> vertex_buffer, unsigned strides, unsigned offsets)
	{
		ID3D11Buffer* _vertexBuffer = reinterpret_cast<ID3D11Buffer*>(vertex_buffer->GetNativePointer());

		m_pDeviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &strides, &offsets);
	}
	void D3D11RenderContext::BindIndexBuffer(Ref<IIndexBuffer> index_buffer, unsigned offset)
	{
		ID3D11Buffer* _indexBuffer = reinterpret_cast<ID3D11Buffer*>(index_buffer->GetNativePointer());

		m_pDeviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, offset);
	}
	void D3D11RenderContext::BindConstantBuffer(Ref<IConstantBuffer> constant_buffer, SHADER_TYPE stage, int slot)
	{
		ID3D11Buffer* _constantBuffer = reinterpret_cast<ID3D11Buffer*>(constant_buffer->GetNativePointer());
		
		switch (stage)
		{
		case SHADER_TYPE::Vertex:
			m_pDeviceContext->VSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		case SHADER_TYPE::Hull:
			m_pDeviceContext->HSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		case SHADER_TYPE::Domain:
			m_pDeviceContext->DSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		case SHADER_TYPE::Geometry:
			m_pDeviceContext->GSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		case SHADER_TYPE::Pixel:
			m_pDeviceContext->PSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		case SHADER_TYPE::Compute:
			m_pDeviceContext->CSSetConstantBuffers(slot, 1, &_constantBuffer);
			break;
		}
	}
	void D3D11RenderContext::BindInputLayout(Ref<IInputLayout> input_layout)
	{
		ID3D11InputLayout* _pInputLayout = reinterpret_cast<ID3D11InputLayout*>(input_layout->GetNativePointer());
		m_pDeviceContext->IASetInputLayout(_pInputLayout);
	}
	void D3D11RenderContext::BindSamplerState(Ref<ISamplerState> sampler_state, unsigned slot)
	{
		ID3D11SamplerState* _pSamplerState = reinterpret_cast<ID3D11SamplerState*>(sampler_state->GetNativePointer());
		m_pDeviceContext->PSSetSamplers(slot, 1, &_pSamplerState);
	}
	void D3D11RenderContext::BindRasterizerState(Ref<IRasterizer> rasterizer_state)
	{
		ID3D11RasterizerState* _pRasterizerState = reinterpret_cast<ID3D11RasterizerState*>(rasterizer_state->GetNativePointer());
		m_pDeviceContext->RSSetState(_pRasterizerState);
	}
	void D3D11RenderContext::BindDepthStencilState(Ref<IDepthStencilState> depth_stencil_state)
	{
		ID3D11DepthStencilState* _pDepthStencilState = reinterpret_cast<ID3D11DepthStencilState*>(depth_stencil_state->GetNativePointer());
		m_pDeviceContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	}
	void D3D11RenderContext::BindTopology(PRIMITIVE_TOPOLOGY topology)
	{
		m_pDeviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
	}
	void D3D11RenderContext::BindTexture2D(Ref<ITexture> texture, SHADER_TYPE stage, unsigned slot)
	{
		ID3D11ShaderResourceView* _pSrv = reinterpret_cast<ID3D11ShaderResourceView*>(texture->GetNativePointer());

		switch (stage)
		{
			case SHADER_TYPE::Vertex:
				m_pDeviceContext->VSSetShaderResources(slot, 1, &_pSrv);
				break;
			case SHADER_TYPE::Hull:
				m_pDeviceContext->HSSetShaderResources(slot, 1, &_pSrv);
				break;
			case SHADER_TYPE::Domain:
				m_pDeviceContext->DSSetShaderResources(slot, 1, &_pSrv);
				break;
			case SHADER_TYPE::Geometry:
				m_pDeviceContext->GSSetShaderResources(slot, 1, &_pSrv);
				break;
			case SHADER_TYPE::Pixel:
				m_pDeviceContext->PSSetShaderResources(slot, 1, &_pSrv);
				break;
			case SHADER_TYPE::Compute:
				m_pDeviceContext->CSSetShaderResources(slot, 1, &_pSrv);
				break;
		}
	}
	void D3D11RenderContext::MapDataToBuffer(Ref<IBuffer> buffer, const void* data, unsigned size)
	{
		ID3D11Buffer* _pBuffer = reinterpret_cast<ID3D11Buffer*>(buffer->GetNativePointer());
		D3D11_MAPPED_SUBRESOURCE _mapData = {};
		WRECK_HR(m_pDeviceContext->Map(_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapData));

		memcpy(_mapData.pData, data, size);

		m_pDeviceContext->Unmap(_pBuffer, 0);
	}
	void D3D11RenderContext::LoadDataFromBuffer(Ref<IBuffer> buffer, void* outputBuffer, unsigned size)
	{
		ID3D11Buffer* _pBuffer = reinterpret_cast<ID3D11Buffer*>(buffer->GetNativePointer());
		D3D11_MAPPED_SUBRESOURCE _mapData = {};
		WRECK_HR(m_pDeviceContext->Map(_pBuffer, 0, D3D11_MAP_READ, 0, &_mapData));

		memcpy(outputBuffer, _mapData.pData, size);

		m_pDeviceContext->Unmap(_pBuffer, 0);
	}
	void D3D11RenderContext::Draw(unsigned vertex_count, unsigned start_vertex_location)
	{
		m_pDeviceContext->Draw(vertex_count, start_vertex_location);
	}
	void D3D11RenderContext::DrawIndexed(unsigned index_count, unsigned start_index_location, unsigned base_vertex_location)
	{
		m_pDeviceContext->DrawIndexed(index_count, start_index_location, base_vertex_location);
	}
	void* D3D11RenderContext::GetNativePointer() const
	{
		return m_pDeviceContext.Get();
	}
}