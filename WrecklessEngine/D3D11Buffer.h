#pragma once

#include "IBuffer.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11VertexBuffer : public IVertexBuffer
	{
	public:
		D3D11VertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer);
		virtual void* GetNativePointer() override;

		virtual ~D3D11VertexBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	};

	class D3D11IndexBuffer : public IIndexBuffer
	{
	public:
		D3D11IndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer);
		virtual void* GetNativePointer() override;

		virtual ~D3D11IndexBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	};

	class D3D11ConstantBuffer : public IConstantBuffer
	{
	public:
		D3D11ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer);
		virtual void* GetNativePointer() override;

		virtual ~D3D11ConstantBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	};
}