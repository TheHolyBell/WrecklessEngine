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
	unsigned D3D11VertexShader::GetByteCodeLength()
	{
		return m_pCompiledShader->GetBufferSize();
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
	unsigned D3D11PixelShader::GetByteCodeLength()
	{
		return m_pCompiledShader->GetBufferSize();
	}
	void* D3D11PixelShader::GetNativePointer()
	{
		return m_pPixelShader.Get();
	}
}