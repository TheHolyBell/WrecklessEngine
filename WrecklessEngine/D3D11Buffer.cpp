#include "D3D11Buffer.h"

namespace Graphics
{
	D3D11VertexBuffer::D3D11VertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer)
		: m_pBuffer(buffer)
	{
	}
	void* D3D11VertexBuffer::GetNativePointer()
	{
		return m_pBuffer.Get();
	}
	D3D11IndexBuffer::D3D11IndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer)
		: m_pBuffer(buffer)
	{
	}
	void* D3D11IndexBuffer::GetNativePointer()
	{
		return m_pBuffer.Get();
	}
	D3D11ConstantBuffer::D3D11ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer)
		: m_pBuffer(buffer)
	{
	}
	void* D3D11ConstantBuffer::GetNativePointer()
	{
		return m_pBuffer.Get();
	}
}