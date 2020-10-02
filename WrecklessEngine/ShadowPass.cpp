#include "ShadowPass.h"

#include "Renderer.h"
#include <wrl/client.h>

#include "D3D11Texture.h"
#include "SceneManager.h"
#include "SceneCamera.h"
#include "Components.h"
#include <entt.hpp>
#include "GlobalClock.h"

namespace Graphics
{
	Ref<IDepthStencilView>	ShadowPass::m_DepthStencil;
	Ref<ITexture>			ShadowPass::m_DepthStencilSRV;
	CameraSystem::OrthogonalCamera ShadowPass::m_LightCamera;

	void ShadowPass::Initialize(unsigned width, unsigned height)
	{
		Resize(width, height);
	}

	void ShadowPass::Resize(unsigned width, unsigned height)
	{
		m_LightCamera.SetFrustumProperties(width, height, 0.5f, 1000);
		/*m_DepthStencilSRV.reset();
		m_DepthStencil.reset();

		m_RenderTargetSRV.reset();
		m_RenderTarget.reset();*/

		
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _DepthTex;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _DepthSRV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DepthDSV;

		ID3D11Device* _pDevice = reinterpret_cast<ID3D11Device*>(Renderer::GetDevice()->GetNativePointer());
		ID3D11DeviceContext* _pDeviceContext = reinterpret_cast<ID3D11DeviceContext*>(Renderer::GetRenderContext()->GetNativePointer());


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
	}

	void ShadowPass::SetLightPosition(DirectX::XMFLOAT3 pos)
	{
		m_LightCamera.SetPosition(pos);
		m_LightCamera.UpdateViewMatrix();
	}

	DirectX::XMFLOAT3 ShadowPass::GetLightPosition()
	{
		return m_LightCamera.GetPosition3f();
	}

	void ShadowPass::Execute()
	{
		Renderer::GetRenderContext()->ClearDepthStencilView(m_DepthStencil, 1.0f);
		Renderer::GetRenderContext()->SetOutputTarget(nullptr, m_DepthStencil);

		float radius = 180;
		float time = Profiling::GlobalClock::TotalTime() * 0.1;
		m_LightCamera.LookAt(DirectX::XMFLOAT3{ radius * cos(time), 20, radius * sin(time) }, {}, { 0,1,0 });
		m_LightCamera.UpdateViewMatrix();

		Viewport vp = {};
		vp.Width = m_DepthStencil->GetWidth();
		vp.Height = m_DepthStencil->GetHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		Renderer::GetRenderContext()->BindViewport(vp);

		DirectX::XMMATRIX prevView = CameraSystem::SceneCamera::GetView();
		DirectX::XMMATRIX prevProj = CameraSystem::SceneCamera::GetProjection();

		CameraSystem::SceneCamera::SetView(m_LightCamera.GetView());
		CameraSystem::SceneCamera::SetProjection(m_LightCamera.GetProjection());

		auto pActiveScene = ECS::SceneManager::GetActiveScene();

		if (pActiveScene != nullptr)
		{
			auto view = pActiveScene->QueryElementsByComponent<ECS::MeshComponent, ECS::ShadowCasterComponent>();
			for (const auto& m : view)
			{
				ECS::MeshComponent& mc = view.get<ECS::MeshComponent>(m);
				mc.Mesh->Update();
				mc.Mesh->Draw();
			}

		}

		CameraSystem::SceneCamera::SetView(prevView);
		CameraSystem::SceneCamera::SetProjection(prevProj);
	}

	DirectX::XMMATRIX ShadowPass::GetShadowTransform()
	{
		return m_LightCamera.GetView() * m_LightCamera.GetProjection();
	}

	Ref<ITexture> ShadowPass::GetDepthStencilSRV()
	{
		return m_DepthStencilSRV;
	}
}