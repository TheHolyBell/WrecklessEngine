#pragma once
#include "WrecklessException.h"
#include <Windows.h>

namespace Exceptions
{
	class HrException : public WrecklessException
	{
	public:
		HrException(const char* fileName, int line, HRESULT hr) noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT m_HR;
	};
}