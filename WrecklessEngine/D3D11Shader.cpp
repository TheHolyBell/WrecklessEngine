#include "D3D11Shader.h"

namespace Graphics
{
	D3D11VertexShader::D3D11VertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader)
		: m_pCompiledShader(compiled_shader), m_pVertexShader(vertex_shader)
	{
	}
	SHADER_TYPE D3D11VertexShader::GetType() const noexcept
	{
		return SHADER_TYPE::Vertex;
	}
	void* D3D11VertexShader::GetByteCode()
	{
		return m_pCompiledShader.Get();
	}
	void* D3D11VertexShader::GetNativePointer()
	{
		return m_pVertexShader.Get();
	}


	D3D11PixelShader::D3D11PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader)
		: m_pCompiledShader(compiled_shader), m_pPixelShader(pixel_shader)
	{
	}
	SHADER_TYPE D3D11PixelShader::GetType() const noexcept
	{
		return SHADER_TYPE::Pixel;
	}
	void* D3D11PixelShader::GetByteCode()
	{
		return m_pCompiledShader.Get();
	}
	void* D3D11PixelShader::GetNativePointer()
	{
		return m_pPixelShader.Get();
	}
	D3D11HullShader::D3D11HullShader(Microsoft::WRL::ComPtr<ID3D11HullShader> hull_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader)
		: m_pHullShader(hull_shader), m_pCompiledShader(compiled_shader)
	{
	}
	SHADER_TYPE D3D11HullShader::GetType() const noexcept
	{
		return SHADER_TYPE::Hull;
	}
	void* D3D11HullShader::GetByteCode()
	{
		return m_pCompiledShader.Get();
	}
	void* D3D11HullShader::GetNativePointer()
	{
		return m_pHullShader.Get();
	}
	D3D11DomainShader::D3D11DomainShader(Microsoft::WRL::ComPtr<ID3D11DomainShader> domain_shader, Microsoft::WRL::ComPtr<ID3DBlob> compiled_shader)
		: m_pDomainShader(domain_shader), m_pCompiledShader(compiled_shader)
	{
	}
	SHADER_TYPE D3D11DomainShader::GetType() const noexcept
	{
		return SHADER_TYPE::Domain;
	}
	void* D3D11DomainShader::GetByteCode()
	{
		return m_pCompiledShader.Get();
	}
	void* D3D11DomainShader::GetNativePointer()
	{
		return m_pDomainShader.Get();
	}
}