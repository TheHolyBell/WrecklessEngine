#pragma once
#include "CommonInclude.h"
#include "ITexture.h"
#include "OrthogonalCamera.h"
#include <DirectXMath.h>

namespace Graphics
{
	class ShadowPass
	{
	public:
		static void Initialize(unsigned width, unsigned height);
		static void Resize(unsigned width, unsigned height);
		//static void Begin(float* color);
		//static void End();

		static void SetLightPosition(DirectX::XMFLOAT3 pos);
		static DirectX::XMFLOAT3 GetLightPosition();

		static void Execute();

		static DirectX::XMMATRIX GetShadowTransform();

		static Ref<ITexture> GetDepthStencilSRV();
	private:
		static Ref<IDepthStencilView> m_DepthStencil;
		static Ref<ITexture> m_DepthStencilSRV;
		static CameraSystem::OrthogonalCamera m_LightCamera;
	};
}