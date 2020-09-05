#include "ScriptField.h"

namespace Scripting
{
	ScriptField::ScriptField(MonoClass* pClass, MonoClassField* pField)
		: m_pClass(pClass), m_pField(pField)
	{
	}
	std::string ScriptField::GetName() const
	{
		return std::string(mono_class_get_name(m_pClass)) + "::" + mono_field_get_name(m_pField);
	}
	const char* ScriptField::GetType()
	{
		return mono_type_get_name(mono_field_get_type(m_pField));
	}
}
