#include "ScriptField.h"

namespace Scripting
{
	ScriptField::ScriptField(MonoObject* pObject, MonoClassField* pField, FieldType type)
		: m_pObject(pObject), m_pField(pField), m_Type(type)
	{
	}
	std::string ScriptField::GetName() const
	{
		return std::string(mono_class_get_name(mono_object_get_class(m_pObject))) + "::" + mono_field_get_name(m_pField);
	}
	FieldType ScriptField::GetType() const
	{
		return m_Type;
	}
}
