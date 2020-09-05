#pragma once
#include <exception>
#include <string>
#include "WrecklessException.h"

#define SCRIPT_ERROR( msg ) throw Scripting::ScriptingException(__FILE__, __LINE__, msg )

namespace Scripting
{
	class ScriptingException : public Exceptions::WrecklessException
	{
	public:
		ScriptingException(const char* file, int line, const std::string& msg);
		virtual const char* what() const override;
		virtual const char* GetType() const noexcept; // @ Used in some cases
	private:
		std::string m_Message;
	};
}