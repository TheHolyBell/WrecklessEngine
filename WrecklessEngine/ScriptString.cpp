#include "ScriptString.h"

namespace Scripting
{
	String::String(MonoString* str)
		: m_pString(str)
	{
		//m_pBuffer = mono_string_to_utf8(m_pString);
	}
	String::String(const char* str)
		: m_pString(mono_string_new(mono_domain_get(), str))
	{
		//m_pBuffer = mono_string_to_utf8(m_pString);
	}
	String::String(const std::string& str)
		: m_pString(mono_string_new(mono_domain_get(), str.c_str()))
	{
		//m_pBuffer = mono_string_to_utf8(m_pString);
	}
	String::String(const String& rhs)
		: m_pString(mono_string_new(mono_domain_get(), rhs.m_pBuffer))
	{
		//m_pBuffer = mono_string_to_utf8(m_pString);
	}
	String& String::operator=(const String& rhs)
	{
		if (this == &rhs)
			return *this;

		m_pString = mono_string_new(mono_domain_get(), rhs.m_pBuffer);
		//m_pBuffer = mono_string_to_utf8(m_pString);

		return *this;
	}
	String::String(String&& rhs)
		: m_pString(rhs.m_pString), m_pBuffer(rhs.m_pBuffer)
	{
		rhs.m_pString = nullptr;
		rhs.m_pBuffer = nullptr;
	}
	String& String::operator=(String&& rhs)
	{
		if (this == &rhs)
			return *this;

		m_pString = rhs.m_pString;
		m_pBuffer = rhs.m_pBuffer;

		rhs.m_pString = nullptr;
		rhs.m_pBuffer = nullptr;

		return *this;
	}
	String::~String()
	{
		mono_free(m_pBuffer);
	}
	int String::Length() const
	{
		return mono_string_length(m_pString);
	}
	const char* String::ToUTF8()
	{
		if (m_pBuffer == nullptr)
			m_pBuffer = mono_string_to_utf8(m_pString);

		return m_pBuffer;
	}
	void* String::ptr() noexcept
	{
		return m_pString;
	}
	MonoString* String::GetString() const
	{
		return m_pString;
	}
}