#include "D3D11DepthStencilState.h"

namespace Graphics
{
	D3D11DepthStencilState::D3D11DepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state)
		: m_pDepthStencilState(depth_stencil_state)
	{
	}
	void* D3D11DepthStencilState::GetNativePointer() const
	{
		return m_pDepthStencilState.Get();
	}
}