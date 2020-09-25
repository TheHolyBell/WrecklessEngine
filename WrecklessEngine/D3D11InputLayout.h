#pragma once

#include "IInputLayout.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11InputLayout : public IInputLayout
	{
	public:
		D3D11InputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout);
		virtual void* GetNativePointer() override;
		virtual unsigned GetElementCount() override;
		virtual ~D3D11InputLayout() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pLayout;
	};
}