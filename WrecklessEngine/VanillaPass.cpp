#include "VanillaPass.h"
#include "Renderer.h"
#include <wrl/client.h>

#include "D3D11Texture.h"
#include "SceneManager.h"
#include "Components.h"
#include <entt.hpp>

namespace Graphics
{
	Ref<IRenderTarget>		VanillaPass::m_RenderTarget;
	Ref<ITexture>			VanillaPass::m_RenderTargetSRV;
	Ref<IDepthStencilView>	VanillaPass::m_DepthStencil;
	Ref<ITexture>			VanillaPass::m_DepthStencilSRV;


	void VanillaPass::Initialize(unsigned width, unsigned height)
	{
		Resize(width, height);
	}

	void VanillaPass::Resize(unsigned width, unsigned height)
	{
		/*m_DepthStencilSRV.reset();
		m_DepthStencil.reset();

		m_RenderTargetSRV.reset();
		m_RenderTarget.reset();*/


		Microsoft::WRL::ComPtr<ID3D11Texture2D> _ColorTex;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _DepthTex;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _ColorSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _DepthSRV;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _ColorRTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DepthDSV;

		ID3D11Device* _pDevice = reinterpret_cast<ID3D11Device*>(Renderer::GetDevice()->GetNativePointer());
		ID3D11DeviceContext* _pDeviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());

		D3D11_TEXTURE2D_DESC _colorDesc = {};
		_colorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;;
		_colorDesc.Width = width;
		_colorDesc.Height = height;
		_colorDesc.MipLevels = 1;
		_colorDesc.ArraySize = 1;
		_colorDesc.SampleDesc.Count = 1;
		_colorDesc.SampleDesc.Quality = 0;
		_colorDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		_colorDesc.Usage = D3D11_USAGE_DEFAULT;
		
		WRECK_HR(_pDevice->CreateTexture2D(&_colorDesc, nullptr, &_ColorTex));
		WRECK_HR(_pDevice->CreateShaderResourceView(_ColorTex.Get(), nullptr, &_ColorSRV));
		WRECK_HR(_pDevice->CreateRenderTargetView(_ColorTex.Get(), nullptr, &_ColorRTV));


		D3D11_TEXTURE2D_DESC _depthDesc = {};
		_depthDesc.Width = width;
		_depthDesc.Height = height;
		_depthDesc.MipLevels = 1;
		_depthDesc.ArraySize = 1;
		_depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		_depthDesc.SampleDesc.Count = 1;
		_depthDesc.SampleDesc.Quality = 0;
		_depthDesc.Usage = D3D11_USAGE_DEFAULT;
		_depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		_depthDesc.CPUAccessFlags = 0;
		_depthDesc.MiscFlags = 0;

		
		WRECK_HR(_pDevice->CreateTexture2D(&_depthDesc, nullptr, &_DepthTex));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		WRECK_HR(_pDevice->CreateDepthStencilView(_DepthTex.Get(), &dsvDesc, &_DepthDSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		WRECK_HR(_pDevice->CreateShaderResourceView(_DepthTex.Get(), &srvDesc, &_DepthSRV));

		m_DepthStencil = std::make_shared<D3D11DepthStencilView>(_DepthDSV);
		m_DepthStencilSRV = std::make_shared<D3D11Texture>(_DepthSRV);

		m_RenderTargetSRV = std::make_shared<D3D11Texture>(_ColorSRV);
		m_RenderTarget = std::make_shared<D3D11RenderTarget>(_ColorRTV);
	}

	/*void VanillaPass::Begin(float* color)
	{
		Renderer::GetRenderContext()->ClearRenderTarget(m_RenderTarget, color);
		Renderer::GetRenderContext()->ClearDepthStencilView(m_DepthStencil, 1.0f);
		Renderer::GetRenderContext()->SetOutputTargets(m_RenderTarget, m_DepthStencil);

		Viewport vp = {};
		vp.Width = m_RenderTarget->GetWidth();
		vp.Height = m_RenderTarget->GetHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		Renderer::GetRenderContext()->BindViewport(vp);
	}

	void VanillaPass::End()
	{
		ID3D11DeviceContext* _pContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());
		ID3D11RenderTargetView* rtv = nullptr;
		_pContext->OMSetRenderTargets(1, &rtv, nullptr);
	}*/

	void VanillaPass::Execute()
	{
		float color[] = { 0,0,0,1 };
		Renderer::GetRenderContext()->ClearRenderTarget(m_RenderTarget, color);
		Renderer::GetRenderContext()->ClearDepthStencilView(m_DepthStencil, 1.0f);
		Renderer::GetRenderContext()->SetOutputTarget(m_RenderTarget, m_DepthStencil);

		Viewport vp = {};
		vp.Width = m_RenderTarget->GetWidth();
		vp.Height = m_RenderTarget->GetHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		Renderer::GetRenderContext()->BindViewport(vp);

		auto pActiveScene = ECS::SceneManager::GetActiveScene();

		if (pActiveScene != nullptr)
		{
			auto cubemapView = pActiveScene->QueryElementsByComponent<ECS::CubemapComponent>();
			if (cubemapView.size() > 1)
				WRECK_ASSERT(false, "Attached more than 1 cubemap");
			for (const auto& cm : cubemapView)
			{
				ECS::CubemapComponent& cc = cubemapView.get<ECS::CubemapComponent>(cm);
				if(cc.Cubemap != nullptr)
					cc.Cubemap->Draw();
			}

			auto view = pActiveScene->QueryElementsByComponent<ECS::MeshComponent>();
			for (const auto& m : view)
			{
				ECS::MeshComponent& mc = view.get<ECS::MeshComponent>(m);
				mc.Mesh->Update();
				mc.Mesh->Draw();
			}

		}
	}

	Ref<ITexture> VanillaPass::GetRenderTargetSRV()
	{
		return m_RenderTargetSRV;
	}

	Ref<ITexture> VanillaPass::GetDepthStencilSRV()
	{
		return m_DepthStencilSRV;
	}

}