#pragma once
#include "CommonInclude.h"
#include "ISamplerState.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11SamplerState : public ISamplerState
	{
	public:
		D3D11SamplerState(Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state);

		virtual void* GetNativePointer() override;

		virtual ~D3D11SamplerState() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
	};
}