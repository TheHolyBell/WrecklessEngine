#pragma once
#include <d3d11.h>
#include <utility>
#include <vector>
#include <string>
#include "Vertex.h"
#include "DynamicConstant.h"

class ShaderReflector
{
public:
	static Dynamic::VertexLayout GetLayoutFromShader(ID3DBlob* shaderByteCode);
	static Dynamic::Buffer GetBufferByRegister(ID3DBlob* shaderByteCode, UINT Register);
	
	static std::vector<int> GetBufferSlots(ID3DBlob* shaderByteCode);
	static std::pair<Dynamic::Buffer, std::vector<std::string>> GetBufferAndNamesByRegister(ID3DBlob* shaderByteCode, UINT Register);
};