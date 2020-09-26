#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include <memory>
#include "CommonInclude.h"
#include <DirectXMath.h>
#include "ITexture.h"
#include "Bindable.h"
#include "GraphicsFormats.h"

namespace Graphics
{
	class IShader
	{
	public:
		IShader() = default;

		IShader(const IShader& rhs) = delete;
		IShader& operator=(const IShader& rhs) = delete;

		IShader(IShader&& rhs) = delete;
		IShader& operator=(IShader&& rhs) = delete;

		virtual ~IShader() = default;

		virtual SHADER_TYPE GetType() const noexcept PURE;

		// @ Currently available for DirectX11
		virtual void* GetByteCode() PURE;
		virtual void* GetNativePointer() PURE;

		// @ Check whether variable exists
		/*virtual bool VariableExists(const std::string& Name, float value) PURE;
		// @ Throws exception if float not exists
		virtual void SetFloat(const std::string& Name, float value) PURE;
		// @ Throws exception if float2 not exists
		virtual void SetFloat2(const std::string& Name, const DirectX::XMFLOAT2& value) PURE;
		// @ Throws exception if float3 not exists
		virtual void SetFloat3(const std::string& Name, const DirectX::XMFLOAT3& value) PURE;
		// @ Throws exception if float4 not exists
		virtual void SetFloat4(const std::string& Name, const DirectX::XMFLOAT4& value) PURE;
		// @ Throws exception if matrix not exists
		virtual void SetMatrix(const std::string& Name, const DirectX::XMMATRIX& matrix) PURE;*/
	};

	class IVertexShader : public IShader
	{
	public:
		virtual ~IVertexShader() = default;
	};

	class IPixelShader : public IShader
	{
	public:
		virtual ~IPixelShader() = default;
	};

	class IHullShader : public IShader
	{
	public:
		virtual ~IHullShader() = default;
	};

	class IDomainShader : public IShader
	{
	public:
		virtual ~IDomainShader() = default;
	};
}