#pragma once
#include "IShader.h"
#include <wrl/client.h>
#include <d3d11.h>

namespace Graphics
{
	class D3D11VertexShader : public IVertexShader
	{
	public:
		D3D11VertexShader(const std::string& path);

	private:
		std::string m_Path;
		std::vector<int> m_RegisterSlots;
		std::unordered_set<std::string> m_LUT; // @ Caching variable names here for better performance in 'VariableExists' method;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCode;
	};

	class D3D11PixelShader : public IPixelShader
	{
	private:
		std::vector<int> m_RegisterSlots;
		std::unordered_set<std::string> m_LUT; // @ Caching variable names here for better performance in 'VariableExists' method;
	};
}