#pragma once
#include "CommonInclude.h"
#include "IDepthStencilState.h"

#include <wrl/client.h>
#include <d3d11.h>

namespace Graphics
{
	class D3D11DepthStencilState : public IDepthStencilState
	{
	public:
		D3D11DepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state);

		virtual void* GetNativePointer() const override;

		virtual ~D3D11DepthStencilState() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	};
}