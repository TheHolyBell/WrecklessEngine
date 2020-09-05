#include "WrecklessException.h"
#include <sstream>

namespace Exceptions
{
	WrecklessException::WrecklessException(const char* fileName, int line) noexcept
		: m_File(fileName), m_Line(line)
	{
	}

	const char* WrecklessException::what() const
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< GetOriginalString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* WrecklessException::GetType() const noexcept
	{
		return "Wreckless Exception";
	}

	const std::string& WrecklessException::GetFileName() const noexcept
	{
		return m_File;
	}

	int WrecklessException::GetLine() const noexcept
	{
		return m_Line;
	}

	std::string WrecklessException::GetOriginalString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << std::endl
			<< "[Line] " << m_Line;
		return oss.str();
	}
}
