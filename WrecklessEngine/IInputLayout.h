#pragma once
#include "CommonInclude.h"


namespace Graphics
{
	class IInputLayout
	{
	public:
		IInputLayout() = default;

		IInputLayout(const IInputLayout& rhs) = delete;
		IInputLayout& operator=(const IInputLayout& rhs) = delete;

		IInputLayout(IInputLayout&& rhs) = delete;
		IInputLayout& operator=(IInputLayout&& rhs) = delete;

		virtual void* GetNativePointer() PURE;
		virtual unsigned GetElementCount() PURE;

		virtual ~IInputLayout() = default;
	};
}