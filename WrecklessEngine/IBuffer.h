#pragma once
#include "CommonInclude.h"
#include "GraphicsFormats.h"

namespace Graphics
{
	class IBuffer
	{
	public:
		IBuffer() = default;

		IBuffer(const IBuffer& rhs) = delete;
		IBuffer& operator=(const IBuffer& rhs) = delete;

		IBuffer(IBuffer&& rhs) = delete;
		IBuffer& operator=(IBuffer&& rhs) = delete;

		virtual void* GetNativePointer() PURE;

		virtual ~IBuffer() = default;
	};

	class IVertexBuffer : public IBuffer
	{
	public:
		IVertexBuffer() = default;

		IVertexBuffer(const IVertexBuffer& rhs) = delete;
		IVertexBuffer& operator=(const IVertexBuffer& rhs) = delete;

		IVertexBuffer(IVertexBuffer&& rhs) = delete;
		IVertexBuffer& operator=(IVertexBuffer&& rhs) = delete;

		virtual ~IVertexBuffer() = default;
	};

	class IIndexBuffer : public IBuffer
	{
	public:
		IIndexBuffer() = default;

		IIndexBuffer(const IIndexBuffer& rhs) = delete;
		IIndexBuffer& operator=(const IIndexBuffer& rhs) = delete;

		IIndexBuffer(IIndexBuffer&& rhs) = delete;
		IIndexBuffer& operator=(IIndexBuffer&& rhs) = delete;

		virtual void* GetNativePointer() PURE;

		virtual ~IIndexBuffer() = default;
	};

	class IConstantBuffer : public IBuffer
	{
	public:
		IConstantBuffer() = default;

		IConstantBuffer(const IConstantBuffer& rhs) = delete;
		IConstantBuffer& operator=(const IConstantBuffer& rhs) = delete;

		IConstantBuffer(IConstantBuffer&& rhs) = delete;
		IConstantBuffer& operator=(IConstantBuffer&& rhs) = delete;

		virtual void* GetNativePointer() PURE;

		virtual ~IConstantBuffer() = default;
	};
}