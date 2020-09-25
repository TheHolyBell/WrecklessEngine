#include "HrException.h"
#include <sstream>
#include <comdef.h>


namespace Exceptions
{

	HrException::HrException(const char* fileName, int line, HRESULT hr) noexcept
		: WrecklessException(fileName, line), m_HR(hr)
	{
	}

	const char* HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginalString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* HrException::GetType() const noexcept
	{
		return "Window Exception";
	}

	HRESULT HrException::GetErrorCode() const noexcept
	{
		return m_HR;
	}

	std::string HrException::GetErrorDescription() const noexcept
	{
		//return TranslateErrorCode(m_HR);
		_com_error err(m_HR);
		return err.ErrorMessage();
	}
}