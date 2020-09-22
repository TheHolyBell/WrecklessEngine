#include "D3D11SamplerState.h"

namespace Graphics
{
	D3D11SamplerState::D3D11SamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state)
		: m_pSamplerState(sampler_state)
	{
	}
	void* D3D11SamplerState::GetNativePointer()
	{
		return m_pSamplerState.Get();
	}
}