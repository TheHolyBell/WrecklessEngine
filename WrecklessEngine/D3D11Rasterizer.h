#pragma once
#include "CommonInclude.h"
#include "IRasterizer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11Rasterizer : public IRasterizer
	{
	public:
		D3D11Rasterizer(Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state);


		virtual ~D3D11Rasterizer() = default;

		virtual void* GetNativePointer() const override;
	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
	};
}