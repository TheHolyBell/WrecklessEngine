
#include "D3D11InputLayout.h"
#include <cassert>

namespace Graphics
{
	D3D11InputLayout::D3D11InputLayout(Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout)
		: m_pLayout(pLayout)
	{
	}
	void* D3D11InputLayout::GetNativePointer()
	{
		return m_pLayout.Get();
	}
	unsigned D3D11InputLayout::GetElementCount()
	{
		assert(false && "Failed to call");
		return 0;
	}
}