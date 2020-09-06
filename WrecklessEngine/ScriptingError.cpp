#include "ScriptingError.h"
#include <sstream>

namespace Scripting
{
	ScriptingException::ScriptingException(const char* file, int line, const std::string& msg)
		: Exceptions::WrecklessException(file, line), m_Message(msg)
	{
	}
	const char* ScriptingException::what() const
	{
		std::ostringstream oss;
		oss << WrecklessException::what();
		oss << std::endl << "[Message] " << m_Message;
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* ScriptingException::GetType() const noexcept
	{
		return "ScriptingException";
	}
}