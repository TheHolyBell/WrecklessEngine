#pragma once

#include "CommonInclude.h"
#include "ITexture.h"

namespace Graphics
{
	class VanillaPass
	{
	public:
		static void Initialize(unsigned width, unsigned height);
		static void Resize(unsigned width, unsigned height);
		//static void Begin(float* color);
		//static void End();

		static void Execute();

		static Ref<ITexture> GetRenderTargetSRV();
		static Ref<ITexture> GetDepthStencilSRV();
	private:
		static Ref<IRenderTarget> m_RenderTarget;
		static Ref<ITexture> m_RenderTargetSRV;
		static Ref<IDepthStencilView> m_DepthStencil;
		static Ref<ITexture> m_DepthStencilSRV;
	};
}