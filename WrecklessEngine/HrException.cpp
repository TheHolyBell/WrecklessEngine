#include "HrException.h"
#include <sstream>

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
		return TranslateErrorCode(m_HR);
	}

	std::string HrException::TranslateErrorCode(HRESULT hr) const noexcept
	{
		char* pMsgBuf = nullptr;
		// windows will allocate memory for err string and make our pointer point to it
		const DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);
		// 0 string length returned indicates a failure
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		// copy error string from windows-allocated buffer to std::string
		std::string errorString = pMsgBuf;
		// free windows buffer
		LocalFree(pMsgBuf);
		return errorString;
	}
}