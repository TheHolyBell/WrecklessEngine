#include "ShaderReflector.h"

#include <wrl\client.h>
#include <d3dcompiler.h>
#include <string>
#include "InputOutputStream.h"

static bool icompare_pred(unsigned char a, unsigned char b)
{
    return std::tolower(a) == std::tolower(b);
}

static bool icompare(const std::string& a, const std::string& b)
{
    if (a.length() == b.length()) {
        return std::equal(b.begin(), b.end(),
            a.begin(), icompare_pred);
    }
    else {
        return false;
    }
}

Dynamic::VertexLayout ShaderReflector::GetLayoutFromShader(ID3DBlob* shaderByteCode)
{
	using Dynamic::VertexLayout;
	VertexLayout _vertexLayout;

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> _pReflector;
	D3DReflect(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(),
		IID_ID3D11ShaderReflection, &_pReflector);

	D3D11_SHADER_DESC _ShaderDesc = {};
	_pReflector->GetDesc(&_ShaderDesc);
    
	auto InputParametersCount = _ShaderDesc.InputParameters;

    DXGI_FORMAT Format;

	for (int i = 0; i < InputParametersCount; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC _inputParameterDesc = {};
		_pReflector->GetInputParameterDesc(i, &_inputParameterDesc);
		std::string _SemanticName(_inputParameterDesc.SemanticName);

        // determine DXGI format
        if (_inputParameterDesc.Mask == 1)
        {
            if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Format = DXGI_FORMAT_R32_UINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Format = DXGI_FORMAT_R32_SINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (_inputParameterDesc.Mask <= 3)
        {
            if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Format = DXGI_FORMAT_R32G32_UINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Format = DXGI_FORMAT_R32G32_SINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (_inputParameterDesc.Mask <= 7)
        {
            if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (_inputParameterDesc.Mask <= 15)
        {
            if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (_inputParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        if (icompare(_SemanticName,"POSITION"))
        {
            if (Format == DXGI_FORMAT_R32G32_FLOAT)
                _vertexLayout.Append(VertexLayout::Position2D);
            else if (Format == DXGI_FORMAT_R32G32B32_FLOAT)
                _vertexLayout.Append(VertexLayout::Position3D);
        }
        else if (icompare(_SemanticName,"NORMAL"))
        {
            if (Format == DXGI_FORMAT_R32G32B32_FLOAT)
                _vertexLayout.Append(VertexLayout::Normal);
        }
        else if (icompare(_SemanticName,"TEXCOORD"))
        {
            if (Format == DXGI_FORMAT_R32G32_FLOAT)
                _vertexLayout.Append(VertexLayout::Texture2D);
        }
        else if (icompare(_SemanticName,"COLOR"))
        {
            if (Format == DXGI_FORMAT_R32G32B32_FLOAT)
                _vertexLayout.Append(VertexLayout::Float3Color);
            else if (Format == DXGI_FORMAT_R32G32B32A32_FLOAT)
                _vertexLayout.Append(VertexLayout::Float4Color);
        }
        else if (icompare(_SemanticName,"BITANGENT"))
        {
            if (Format == DXGI_FORMAT_R32G32B32_FLOAT)
                _vertexLayout.Append(VertexLayout::Bitangent);
        }
        else if (icompare(_SemanticName,"TANGENT"))
        {
            if (Format == DXGI_FORMAT_R32G32B32_FLOAT)
                _vertexLayout.Append(VertexLayout::Tangent);
        }
	}

	return _vertexLayout;
}

Dynamic::Buffer ShaderReflector::GetBufferByRegister(ID3DBlob* shaderByteCode, UINT Register)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> _pReflector;
    Dynamic::RawLayout _Layout;

    if (FAILED(D3DReflect(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, &_pReflector)))
    {
        assert(false && "Failed to reflect shader");
    }

    D3D11_SHADER_DESC _shaderDesc = {};

    _pReflector->GetDesc(&_shaderDesc);



    ID3D11ShaderReflectionConstantBuffer* _cbuffer = nullptr;
    D3D11_SHADER_BUFFER_DESC _bufferDesc = {};

    for (int i = 0; i < _shaderDesc.ConstantBuffers; ++i)
    {
        auto* _buff = _pReflector->GetConstantBufferByIndex(i);

        _buff->GetDesc(&_bufferDesc);

        D3D11_SHADER_INPUT_BIND_DESC _bindDesc = {};
        _pReflector->GetResourceBindingDescByName(_bufferDesc.Name, &_bindDesc);
        if (_bindDesc.BindPoint == Register)
        {
            _cbuffer = _buff;
            break;
        }
    }

    assert(_cbuffer != nullptr && "Failed to find CBuffer");


    for (int i = 0; i < _bufferDesc.Variables; ++i)
    {
        auto _variable = _cbuffer->GetVariableByIndex(i);

        D3D11_SHADER_VARIABLE_DESC _varDesc = {};
        _variable->GetDesc(&_varDesc);
        
        auto _Type = _variable->GetType();

        D3D11_SHADER_TYPE_DESC _typeDesc = {};
        _Type->GetDesc(&_typeDesc);

        std::string _variableType = _typeDesc.Name;

        //IO::cout << "Current variable: " << _variableType << " " << _varDesc.Name << IO::endl;
        if (_variableType == "float")
            _Layout.Add<Dynamic::Float>(_varDesc.Name);
        else if (_variableType == "float2")
            _Layout.Add<Dynamic::Float2>(_varDesc.Name);
        else if (_variableType == "float3")
            _Layout.Add<Dynamic::Float3>(_varDesc.Name);
        else if (_variableType == "float4")
            _Layout.Add<Dynamic::Float4>(_varDesc.Name);
        else if (_variableType == "float4x4")
            _Layout.Add<Dynamic::Matrix>(_varDesc.Name);
        else if (_variableType == "bool")
            _Layout.Add<Dynamic::Bool>(_varDesc.Name);
        else if (_variableType == "uint")
            _Layout.Add<Dynamic::Integer>(_varDesc.Name);

    }

    IO::cout << "\n\n";

    return Dynamic::Buffer(std::move(_Layout));
}

std::vector<int> ShaderReflector::GetBufferSlots(ID3DBlob* shaderByteCode)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> _pReflector;
    std::vector<int> _Slots;

    if (FAILED(D3DReflect(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, &_pReflector)))
    {
        assert(false && "Failed to reflect shader");
    }

    D3D11_SHADER_DESC _shaderDesc = {};

    _pReflector->GetDesc(&_shaderDesc);



    ID3D11ShaderReflectionConstantBuffer* _cbuffer = nullptr;
    D3D11_SHADER_BUFFER_DESC _bufferDesc = {};

    for (int i = 0; i < _shaderDesc.ConstantBuffers; ++i)
    {
        auto* _buff = _pReflector->GetConstantBufferByIndex(i);

        _buff->GetDesc(&_bufferDesc);

        D3D11_SHADER_INPUT_BIND_DESC _bindDesc = {};
        _pReflector->GetResourceBindingDescByName(_bufferDesc.Name, &_bindDesc);
        _Slots.push_back(_bindDesc.BindPoint);
    }

    return _Slots;
}

std::pair<Dynamic::Buffer, std::vector<std::string>> ShaderReflector::GetBufferAndNamesByRegister(ID3DBlob* shaderByteCode, UINT Register)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderReflection> _pReflector;
    Dynamic::RawLayout _Layout;
    std::vector<std::string> _Names;

    if (FAILED(D3DReflect(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), IID_ID3D11ShaderReflection, &_pReflector)))
    {
        assert(false && "Failed to reflect shader");
    }

    D3D11_SHADER_DESC _shaderDesc = {};

    _pReflector->GetDesc(&_shaderDesc);

    ID3D11ShaderReflectionConstantBuffer* _cbuffer = nullptr;
    D3D11_SHADER_BUFFER_DESC _bufferDesc = {};

    for (int i = 0; i < _shaderDesc.ConstantBuffers; ++i)
    {
        auto* _buff = _pReflector->GetConstantBufferByIndex(i);

        _buff->GetDesc(&_bufferDesc);

        D3D11_SHADER_INPUT_BIND_DESC _bindDesc = {};
        _pReflector->GetResourceBindingDescByName(_bufferDesc.Name, &_bindDesc);
        if (_bindDesc.BindPoint == Register)
        {
            _cbuffer = _buff;
            break;
        }
    }

    assert(_cbuffer != nullptr && "Failed to find CBuffer");


    for (int i = 0; i < _bufferDesc.Variables; ++i)
    {
        auto _variable = _cbuffer->GetVariableByIndex(i);

        D3D11_SHADER_VARIABLE_DESC _varDesc = {};
        _variable->GetDesc(&_varDesc);

        auto _Type = _variable->GetType();

        D3D11_SHADER_TYPE_DESC _typeDesc = {};
        _Type->GetDesc(&_typeDesc);

        std::string _variableType = _typeDesc.Name;

        if (_variableType == "float")
            _Layout.Add<Dynamic::Float>(_varDesc.Name);
        else if (_variableType == "float2")
            _Layout.Add<Dynamic::Float2>(_varDesc.Name);
        else if (_variableType == "float3")
            _Layout.Add<Dynamic::Float3>(_varDesc.Name);
        else if (_variableType == "float4")
            _Layout.Add<Dynamic::Float4>(_varDesc.Name);
        else if (_variableType == "float4x4")
            _Layout.Add<Dynamic::Matrix>(_varDesc.Name);
        else if (_variableType == "bool")
            _Layout.Add<Dynamic::Bool>(_varDesc.Name);
        else if (_variableType == "uint")
            _Layout.Add<Dynamic::Integer>(_varDesc.Name);

        _Names.emplace_back(_varDesc.Name);
    }

    return { Dynamic::Buffer(std::move(_Layout)), _Names };
}
