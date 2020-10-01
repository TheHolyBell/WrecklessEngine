#pragma once
#include "IShader.h"
#include <wrl/client.h>
#include <d3d11.h>

namespace Graphics
{
	class D3D11VertexShader : public IVertexShader
	{
	public:
		D3D11VertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader);

		virtual SHADER_TYPE GetType() const noexcept override;

		// @ Currently available for DirectX11
		virtual void* GetByteCode() override;
		virtual void* GetNativePointer() override;

		virtual ~D3D11VertexShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pCompiledShader;
	};

	class D3D11HullShader : public IHullShader
	{
	public:
		D3D11HullShader(Microsoft::WRL::ComPtr<ID3D11HullShader> hull_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader);

		virtual SHADER_TYPE GetType() const noexcept override;

		// @ Currently available for DirectX11
		virtual void* GetByteCode() override;
		virtual void* GetNativePointer() override;

		virtual ~D3D11HullShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11HullShader> m_pHullShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pCompiledShader;
	};

	class D3D11DomainShader : public IDomainShader
	{
	public:
		D3D11DomainShader(Microsoft::WRL::ComPtr<ID3D11DomainShader> domain_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader);

		virtual SHADER_TYPE GetType() const noexcept override;

		// @ Currently available for DirectX11
		virtual void* GetByteCode() override;
		virtual void* GetNativePointer() override;

		virtual ~D3D11DomainShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11DomainShader> m_pDomainShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pCompiledShader;
	};

	class D3D11PixelShader : public IPixelShader
	{
	public:
		D3D11PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader);

		virtual SHADER_TYPE GetType() const noexcept override;

		// @ Currently available for DirectX11
		virtual void* GetByteCode() override;
		virtual void* GetNativePointer() override;

		virtual ~D3D11PixelShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pCompiledShader;
	};
}