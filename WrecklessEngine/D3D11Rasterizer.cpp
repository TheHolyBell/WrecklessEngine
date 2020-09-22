#include "D3D11Rasterizer.h"

namespace Graphics
{
	D3D11Rasterizer::D3D11Rasterizer(Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state)
		: m_pRasterizerState(rasterizer_state)
	{
	}
	void* D3D11Rasterizer::GetNativePointer() const
	{
		return m_pRasterizerState.Get();
	}
}